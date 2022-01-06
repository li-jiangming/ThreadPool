/*
 * thread_pool.h
 *
 *  Created on: 2022年1月6日
 *      Author: ljm
 */

#include "ab_thread_pool.h"

#include "ab_assert.h"
#include "ab_mem.h"
#include "ab_queue.h"

#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

#define T ab_thread_pool_t

typedef struct ab_thread_pool_task_t {
    void (*func)(void *);
    void *data;
} *ab_thread_pool_task_t;

struct T {
    ab_queue_t task_queue;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int thd_nums;
    bool quit;
    pthread_t *thds;
};

static void *start_routine(void *arg) {
    T tp = (T) arg;
    while (true) {
        ab_thread_pool_task_t task = NULL;
        pthread_mutex_lock(&tp->mutex);
        if (!tp->quit && ab_queue_length(tp->task_queue) == 0)
            pthread_cond_wait(&tp->cond, &tp->mutex);
        task = ab_queue_pop(tp->task_queue);
        pthread_mutex_unlock(&tp->mutex);
        if (NULL == task)
            break;
        task->func(task->data);
    }
    return NULL;
}

T ab_thread_pool_new(int nums) {
    T result;
    NEW(result);
    assert(result);
    result->task_queue = ab_queue_new();
    pthread_mutex_init(&result->mutex, NULL);
    pthread_cond_init(&result->cond, NULL);
    result->thd_nums = nums;
    result->quit = false;
    result->thds = (pthread_t *) ALLOC(nums * sizeof(pthread_t));
    for (int i = 0; i < nums; i++)
        pthread_create(&result->thds[i], NULL, start_routine, result);
    return result;
}

void ab_thread_pool_free(T *tp) {
    assert(tp && *tp);
    pthread_mutex_lock(&(*tp)->mutex);
    (*tp)->quit = true;
    pthread_mutex_unlock(&(*tp)->mutex);
    pthread_cond_broadcast(&(*tp)->cond);
    for (int i = 0; i < (*tp)->thd_nums; i++)
        pthread_join((*tp)->thds[i], NULL);
    pthread_mutex_destroy(&(*tp)->mutex);
    pthread_cond_destroy(&(*tp)->cond);
    ab_queue_free(&(*tp)->task_queue);
    FREE((*tp)->thds);
    FREE(*tp);
}

void ab_thread_pool_add_task(T tp, void func(void*), void *data) {
    assert(tp);
    assert(func);

    ab_thread_pool_task_t task;
    NEW(task);
    assert(task);
    task->func = func;
    task->data = data;

    pthread_mutex_lock(&tp->mutex);
    ab_queue_push(tp->task_queue, task);
    pthread_mutex_unlock(&tp->mutex);
    pthread_cond_signal(&tp->cond);
}
