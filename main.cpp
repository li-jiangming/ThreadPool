#include "threadpool.h"

#include <unistd.h>
#include <iostream>

int main(int argc, char *argv[]) {
    ljm::ThreadPool thread_pool(10);
    thread_pool.start();

    for (int i = 0; i < 100; i++) {
        auto res = thread_pool.addTask([](int num)->int{
            std::cout << "nums: " << num << std::endl;
            return num;
        }, i);

        std::cout << "return num: " << res.get() << std::endl;
    }

    sleep(1);
    thread_pool.stop();

    return EXIT_SUCCESS;
}