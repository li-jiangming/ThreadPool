/*
 * ab_threadpool.c
 *
 *  Created on: 2022年1月6日
 *      Author: ljm
 */

#include "ab_threadpool.h"

#include "ab_assert.h"
#include "ab_mem.h"
#include "ab_queue.h"

#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

#define T ab_threadpool_t

typedef struct ab_threadpool_task_t {
    void (*func)(void *);
    void *data;
} *ab_threadpool_task_t;

struct T {
    ab_queue_t task_queue;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    uint16_t thd_nums;
    bool quit;
    pthread_t *thds;
};

static void *start_routine(void *arg) {
    T tp = (T) arg;
    while (true) {
        ab_threadpool_task_t task = NULL;
        pthread_mutex_lock(&tp->mutex);
        if (!tp->quit && ab_queue_length(tp->task_queue) == 0) {
            pthread_cond_wait(&tp->cond, &tp->mutex);
        }
        
        task = ab_queue_pop(tp->task_queue);
        pthread_mutex_unlock(&tp->mutex);
        if (NULL == task) {
            break;
        }

        task->func(task->data);
    }

    return NULL;
}

T ab_threadpool_new() {
    T result;
    NEW(result);
    if (result != NULL) {
        result->task_queue = ab_queue_new();
        result->thd_nums = 0;
        result->quit = true;
        result->thds = NULL;
    }

    return result;
}

void ab_threadpool_free(T *t) {
    assert(t && *t);
    ab_queue_free(&(*t)->task_queue);
    FREE(*t);
}

void ab_threadpool_set_nums(T t, uint16_t thd_nums) {
    assert(t);

    t->thd_nums = thd_nums;
}

int32_t ab_threadpool_start(T t) {
    assert(t);

    int32_t result = -1;

    pthread_mutex_init(&t->mutex, NULL);
    pthread_cond_init(&t->cond, NULL);
    t->quit = false;
    if (t->thd_nums > 0) {
        t->thds = (pthread_t *) ALLOC(t->thd_nums * sizeof(pthread_t));
        for (int i = 0; i < t->thd_nums; i++) {
            pthread_create(&t->thds[i], NULL, start_routine, t);
        }
    }

    return result;
}

void ab_threadpool_stop(T t) {
    assert(t);

    pthread_mutex_lock(&t->mutex);
    t->quit = true;
    pthread_mutex_unlock(&t->mutex);
    pthread_cond_broadcast(&t->cond);

    for (int i = 0; i < t->thd_nums; ++i) {
        pthread_join(t->thds[i], NULL);
    }

    FREE(t->thds);

    pthread_mutex_destroy(&t->mutex);
    pthread_cond_destroy(&t->cond);
}

void ab_threadpool_add_task(T t, void func(void*), void *data) {
    assert(t);
    assert(func);

    ab_threadpool_task_t task;
    NEW(task);
    if (task != NULL) {
        task->func = func;
        task->data = data;

        pthread_mutex_lock(&t->mutex);
        ab_queue_push(t->task_queue, task);
        pthread_mutex_unlock(&t->mutex);
        pthread_cond_signal(&t->cond);
    }
}
