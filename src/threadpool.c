#include <errno.h>
#include <string.h>

#include <pthread.h>

#include <sutil/dmem.h>
#include <sutil/queue.h>

#include "threadpool.h"

struct threadpool {
  size_t thread_count;
  pthread_t *threads;
  queue_t *queue;
  bool shutdown;

  pthread_mutex_t lock[1];
  pthread_cond_t notify[1];
};

static void *worker(void *arg) {
  threadpool_t *tp = (threadpool_t *)arg;
  threadpool_task_t *task;

  while(1) {
    pthread_mutex_lock(tp->lock);
    
    while(queue_length(tp->queue) == 0 && !tp->shutdown)
      pthread_cond_wait(tp->notify, tp->lock);

    if(tp->shutdown) {
      pthread_mutex_unlock(tp->lock);
      return NULL;
    }

    task = (threadpool_task_t *)queue_poll(tp->queue);
    pthread_mutex_unlock(tp->lock);

    task->func(task->arg);
  }
}


static threadpool_t *threadpool_init(size_t threads) {
  threadpool_t *tp = (threadpool_t *) ualloc(sizeof(*tp));
  if(tp == NULL) {
    errno = ENOMEM;
    return NULL;
  }

  if((tp->threads = (pthread_t *)ualloc(sizeof(*tp->threads) * threads)) == NULL) {
    errno = ENOMEM;
    goto _error;
  }

  if((tp->queue = queue_new_uncapped())) {
    errno = ENOMEM;
    goto _error;
  }

  tp->shutdown = false;
  tp->thread_count = threads;
  pthread_mutex_init(tp->lock, NULL);
  pthread_cond_init(tp->notify, NULL);

  for(size_t i = 0; i < threads; i++)
    pthread_create(tp->threads + i, NULL, worker, (void *) tp);
    

  return tp;

  _error: {
    errno_t tmp = errno;
    ufree(tp->threads);
    queue_destroy(tp->queue);
    errno = tmp;
    return NULL;
  }
}

static int threadpool_submit(threadpool_t *tp, threadpool_task_t _task) {
  pthread_mutex_lock(tp->lock);
  threadpool_task_t *task = (threadpool_task_t *) ualloc(sizeof(*task));
  if(task == NULL) {
    errno = ENOMEM;
    goto _error;
  }

  memcpy(task, &_task, sizeof(*task));
  if(queue_add(tp->queue, (void *)task) < 0)
    goto _error;

  pthread_cond_signal(tp->notify);
  pthread_mutex_unlock(tp->lock);
  return 0;

  _error: {
    errno_t tmp = errno;
    pthread_mutex_unlock(tp->lock);
    errno = tmp;
    return -1;
  }
}

static void threadpool_destroy(threadpool_t *tp) {
  if(tp == NULL)
    return;

  tp->shutdown = true;
  pthread_cond_broadcast(tp->notify);

  for(size_t i = 0; i < tp->thread_count; i++)
    pthread_join(tp->threads[i], NULL);

  void *item;
  while((item = queue_poll(tp->queue)) != NULL)
    ufree(item);

  queue_destroy(tp->queue);
  ufree(tp->threads);
  pthread_mutex_destroy(tp->lock);
  pthread_cond_destroy(tp->notify);
  ufree(tp);
}

const threadpool_methods_t threadpool = {
  threadpool_init,
  threadpool_submit,
  threadpool_destroy
};

