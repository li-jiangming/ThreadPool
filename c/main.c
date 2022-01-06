/*
 * main.c
 *
 *  Created on: 2022年1月6日
 *      Author: ljm
 */

#include "ab_queue.h"
#include "ab_stack.h"
#include "ab_assert.h"
#include "ab_thread_pool.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void apply(void **x, void *cl) {
    assert(x && *x);

    int *num = *x;
    printf("num = %d\n", *num);
}

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

    printf("queue**********************\n");

    ab_queue_t queue = ab_queue_new();

    ab_queue_push(queue, &num1);
    ab_queue_push(queue, &num2);
    ab_queue_push(queue, &num3);
    ab_queue_push(queue, &num4);
    ab_queue_push(queue, &num5);

    ab_queue_map(queue, apply, NULL);
    ab_queue_pop(queue);
    ab_queue_map(queue, apply, NULL);

    ab_queue_free(&queue);

    printf("stack**********************\n");
    ab_stack_t stack = ab_stack_new();

    ab_stack_push(stack, &num1);
    ab_stack_push(stack, &num2);
    ab_stack_push(stack, &num3);
    ab_stack_push(stack, &num4);
    ab_stack_push(stack, &num5);

    ab_stack_map(stack, apply, NULL);
    ab_stack_pop(stack);
    ab_stack_map(stack, apply, NULL);

    ab_stack_free(&stack);

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
