#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#ifdef __cplusplus__
extern "C" {
#endif
   
#define T ab_thread_pool_t
typedef struct T *T;

extern T    ab_thread_pool_new(int thd_nums);
extern void ab_thread_pool_free(T *tp);

extern void ab_thread_pool_add_task(T tp, void func(void *), void *data);

#undef T

#ifdef __cplusplus__
}
#endif

#endif /* THREAD_POOL_H_ */
