/*
 * ab_queue.c
 *
 *  Created on: 2022年1月6日
 *      Author: ljm
 */

#include "ab_queue.h"

#include "ab_list_node.h"
#include "ab_assert.h"
#include "ab_mem.h"

#include <stdlib.h>

#define T ab_queue_t

struct T {
    ab_list_node_t head, tail;
};

T ab_queue_new() {
    T result;
    NEW(result);
    assert(result);
    result->head = result->tail = NULL;
    return result;
}

void ab_queue_free(T *queue) {
    assert(queue && *queue);
    while ((*queue)->head) {
        ab_queue_pop(*queue);
    }
    FREE(*queue);
}

void ab_queue_push(T queue, void *x) {
    assert(queue);
    assert(x);

    ab_list_node_t list_node;
    NEW(list_node);

    assert(list_node);

    list_node->last = list_node->next = NULL;
    list_node->data = x;

    if (queue->tail) {
        queue->tail->next = list_node;
        list_node->last = queue->tail;
        queue->tail = list_node;
    } else {
        queue->head = queue->tail = list_node;
    }
}

void *ab_queue_pop(T queue) {
    assert(queue);
    void *result = NULL;
    ab_list_node_t list_node = queue->head;
    if (list_node) {
        queue->head = list_node->next;
        if (queue->head)
            queue->head->last = NULL;
        else
            queue->tail = NULL;
        result = list_node->data;
        FREE(list_node);
    }
    return result;
}

int ab_queue_length(T queue) {
    assert(queue);
    int result;
    ab_list_node_t list_node = queue->head;
    for (result = 0; list_node; list_node = list_node->next)
        result++;
    return result;
}

void ab_queue_map(T queue, void apply(void **x, void *cl), void *cl) {
    assert(queue);
    assert(apply);
    ab_list_node_t list_node = queue->head;
    for (; list_node; list_node = list_node->next)
        apply(&list_node->data, cl);
}

void **ab_queue_to_array(T queue, void *end) {
    assert(queue);
    int i, n = ab_queue_length(queue);
    void **result = ALLOC((n + 1) * sizeof(*result));
    ab_list_node_t list_node = queue->head;
    for (i = 0; list_node; list_node = list_node->next)
        result[i++] = list_node->data;
    result[i] = end;
    return result;
}
