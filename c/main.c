/*
 * main.c
 *
 *  Created on: 2022年1月6日
 *      Author: ljm
 */

#include "ab_assert.h"
#include "ab_thread_pool.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void task_func(void *data) {
    assert(data);
    int *num = (int *) data;
    printf("num = %d\n", *num);
}

int main(int argc, char *argv[]) {
    int num1 = 1;
    int num2 = 2;
    int num3 = 3;
    int num4 = 4;
    int num5 = 5;

    printf("threadpool**********************\n");
    ab_thread_pool_t tp = ab_thread_pool_new(10);

    ab_thread_pool_add_task(tp, task_func, &num1);
    ab_thread_pool_add_task(tp, task_func, &num2);
    ab_thread_pool_add_task(tp, task_func, &num3);
    ab_thread_pool_add_task(tp, task_func, &num4);
    ab_thread_pool_add_task(tp, task_func, &num5);

    ab_thread_pool_free(&tp);

    return EXIT_SUCCESS;
}
