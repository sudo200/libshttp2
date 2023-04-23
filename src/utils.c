#include <unistd.h>

#include <pthread.h>

#include "utils.h"

int sigevent_respond(sigevent_t *e) {
  if(e == NULL)
    return -1;

  switch (e->sigev_notify) {
    case SIGEV_SIGNAL: {
      return sigqueue(getpid(), e->sigev_signo, e->sigev_value);
    } break;

    case SIGEV_THREAD: {
      return pthread_create(
          (pthread_t *)&e->_sigev_un._tid,
          e->_sigev_un._sigev_thread._attribute,
          (void *(*)(void *))e->_sigev_un._sigev_thread._function,
          *(void **)&e->sigev_value
          );
    } break;

    case SIGEV_NONE:
      break;

    default:
      return -1;
  }

  return 0;
}

