/*
 * ab_list_node.h
 *
 *  Created on: 2022年1月6日
 *      Author: ljm
 */

#ifndef AB_LIST_NODE_H_
#define AB_LIST_NODE_H_

#ifdef __cplusplus
extern "C" {
#endif

#define T ab_list_node_t
typedef struct T *T;

struct T {
    T last, next;
    void *data;
};

#undef T

#ifdef __cplusplus
}
#endif

#endif /* AB_LIST_NODE_H_ */
