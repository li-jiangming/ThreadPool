/*
 * ab_threadpool.h
 *
 *  Created on: 2022年1月6日
 *      Author: ljm
 */

#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define T ab_threadpool_t
typedef struct T *T;

extern T        ab_threadpool_new();
extern void     ab_threadpool_free(T *t);

extern void     ab_threadpool_set_nums(T t, uint16_t thd_nums);

extern int32_t  ab_threadpool_start(T t);
extern void     ab_threadpool_stop(T t);

extern void     ab_threadpool_add_task(T t, void func(void *), void *data);

#undef T

#ifdef __cplusplus
}
#endif

#endif /* THREADPOOL_H_ */
