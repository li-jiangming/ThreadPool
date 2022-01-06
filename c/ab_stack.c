/*
 * ab_stack.c
 *
 *  Created on: 2022年1月6日
 *      Author: ljm
 */

#include "ab_stack.h"

#include "ab_list_node.h"
#include "ab_assert.h"
#include "ab_mem.h"

#include <stdlib.h>

#define T ab_stack_t

struct T {
    ab_list_node_t head;
};

T ab_stack_new() {
    T stack;
    NEW(stack);

    assert(stack);

    stack->head = NULL;
    return stack;
}

void ab_stack_free(T *stack) {
    assert(stack && *stack);

    while ((*stack)->head) {
        ab_stack_pop(*stack);
    }

    FREE(*stack);
}

void ab_stack_push(T stack, void *x) {
    assert(stack);
    assert(x);

    ab_list_node_t list_node;
    NEW(list_node);

    assert(list_node);

    list_node->last = NULL;
    list_node->data = x;

    list_node->next = stack->head;

    stack->head = list_node;
}

void *ab_stack_pop(T stack) {
    assert(stack);

    void *result = NULL;

    ab_list_node_t list_node = stack->head;
    if (list_node) {
        stack->head = list_node->next;
        result = list_node->data;
        FREE(list_node);
    }

    return result;
}

int ab_stack_length(T stack) {
    assert(stack);

    int result;
    ab_list_node_t list_node = stack->head;
    for (result = 0; list_node; list_node = list_node->next)
        result++;
    return result;
}

void ab_stack_map(T stack, void apply(void **x, void *cl), void *cl) {
    assert(stack);
    assert(apply);

    ab_list_node_t list_node = stack->head;
    for (; list_node; list_node = list_node->next)
        apply(&list_node->data, cl);
}

void **ab_stack_to_array(T stack, void *end) {
    assert(stack);

    int i, n = ab_stack_length(stack);
    void **result = ALLOC((n + 1) + sizeof(*result));
    ab_list_node_t list_node = stack->head;
    for (i = 0; list_node; list_node = list_node->next)
        result[i++] = list_node->data;
    result[i] = end;
    return result;
}

