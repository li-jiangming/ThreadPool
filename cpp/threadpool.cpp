#include "threadpool.h"

namespace ljm {

ThreadPool::ThreadPool(int thd_nums)
    : quit_(true),
    thd_nums_(thd_nums) {
}

ThreadPool::~ThreadPool() {}

bool ThreadPool::start() {
    quit_ = false;
    for (int i = 0; i < thd_nums_; i++) {
        workers_.emplace_back([this] {
            while (!quit_) {
                std::function<void()> task;

                {
                    std::unique_lock<std::mutex> locker(mutex_);
                    cond_.wait(locker, [this] {
                        return quit_ || !tasks_.empty();
                    });

                    if (quit_ && tasks_.empty()) {
                        break;
                    }

                    task = std::move(tasks_.front());
                    tasks_.pop();
                }

                task();
            }
        });
    }

    return true;
}

void ThreadPool::stop() {
    {
        std::unique_lock<std::mutex> locker(mutex_);
        quit_ = true;
    }

    cond_.notify_all();
    for (std::thread &worker : workers_)
        worker.join();

}

}