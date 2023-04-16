#include <assert.h>
#include <stdio.h>
#include <unistd.h>

#include "threadpool.h"

#define LEN(arr)  (sizeof(arr)/sizeof(*arr))

static __attribute__((used)) void task(void *arg __attribute__((unused))) {
  sleep(1);
  printf("%p: %s\n", arg, (const char *)arg);
}

int main(void) {
  static const char *const strs[] __attribute__((used)) = {
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "10"
  };

  threadpool_t *pool = threadpool.init(3);
  assert(pool != NULL);

  for(size_t i = 0; i < LEN(strs); i++)
    threadpool.submit(pool, (threadpool_task_t) {
      .func = task,
      .arg = (void *)strs[i],
    });

  sleep(6);

  for(size_t i = 0; i < LEN(strs); i++)
    threadpool.submit(pool, (threadpool_task_t) {
      .func = task,
      .arg = (void *)strs[i]
    });

  sleep(7);

  threadpool.destroy(pool);
  return 0;
}

