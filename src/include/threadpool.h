#ifndef ___THREADPOOL_H__
#define ___THREADPOOL_H__

#include <signal.h>

#include <sutil/types.h>

#define NONNULL(...)  __attribute__((nonnull(__VA_ARGS__)))

typedef union {
  long num;
  const void *cptr;
  void *ptr;
} threadpool_task_arg_t;

typedef struct {
  void (*func)(threadpool_task_arg_t);
  threadpool_task_arg_t arg;
} threadpool_task_t;

typedef struct threadpool threadpool_t;

typedef struct {
  threadpool_t *(*init)(size_t threads);
  int (*submit)(threadpool_t *tp, threadpool_task_t task) NONNULL(1);
  void (*destroy)(threadpool_t *tp);
} threadpool_methods_t;

extern const threadpool_methods_t threadpool;

#undef  NONNULL

#endif  //___THREADPOOL_H__

