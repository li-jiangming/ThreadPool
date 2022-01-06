#if !defined(THREAD_POOL_H_)
#define THREAD_POOL_H_

#include <vector>
#include <queue>
#include <future>

#include <functional>

#include <condition_variable>
#include <mutex>
#include <thread>

namespace ljm {

class ThreadPool {
public:
    ThreadPool(int thd_nums);
    ~ThreadPool();

    bool start();
    void stop();

    template<class F, class... Args>
    auto addTask(F&& f, Args&&... args)->std::future<decltype(f(args...))> {
        using return_type = decltype(f(args...));
        auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        std::future<return_type> res = task->get_future();

        {
            std::unique_lock<std::mutex> locker(mutex_);
            tasks_.emplace([task]() {
                (*task)(); 
            });
        }

        cond_.notify_one();

        return res;
    }

private:
    bool quit_;
    int thd_nums_;
    std::condition_variable cond_;
    std::mutex mutex_;

    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
};

}

#endif // THREAD_POOL_H_