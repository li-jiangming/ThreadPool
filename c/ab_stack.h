/*
 * ab_stack.h
 *
 * Created on: 2022年1月6日
 *     Author: ljm
 */

#ifndef AB_STACK_H_
#define AB_STACK_H_

#ifdef __cplusplus__
extern "C" {
#endif

#define T ab_stack_t
typedef struct T *T;

extern T        ab_stack_new();
extern void     ab_stack_free(T *stack);

extern void     ab_stack_push(T stack, void *x);
extern void    *ab_stack_pop(T stack);

extern int      ab_tack_length(T stack);

extern void     ab_stack_map(T stack, void apply(void **x, void *cl), void *cl);
extern void   **ab_stack_to_array(T stack, void *end);

#undef T

#ifdef __cplusplus__
}
#endif

#endif /* AB_STACK_H_ */
