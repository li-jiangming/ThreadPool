/*
 * ab_queue.h
 *
 * Created on: 2022年1月6日
 *     Author: ljm
 */

#ifndef AB_QUEUE_H_
#define AB_QUEUE_H_

#ifdef __cplusplus__
extern "C" {
#endif

#define T ab_queue_t
typedef struct T *T;

extern T        ab_queue_new();
extern void     ab_queue_free(T *queue);

extern void     ab_queue_push(T queue, void *x);
extern void    *ab_queue_pop(T queue);

extern int      ab_queue_length(T queue);

extern void     ab_queue_map(T queue, void apply(void **x, void *cl), void *cl);

extern void   **ab_queue_to_array(T queue, void *end);

#undef T

#ifdef __cplusplus__
}
#endif

#endif /* AB_QUEUE_H_ */
