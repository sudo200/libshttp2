#include <errno.h>
#include <string.h> // for memcpy
#include <unistd.h>

#include <poll.h>

#include <sutil/dmem.h>
#include <sutil/network.h>

#include "server_struct.h"

static inline bool check_running(http_server_t *srv) {
  pthread_mutex_lock(srv->running_lock);
  bool __running = srv->running;
  pthread_mutex_unlock(srv->running_lock);
  return __running;
}

static void destructor_cb(void *ptr, void *pipe __attribute__((unused))) {
  ufree(ptr);
}


static http_server_t *http_server_init(http_server_handler_t handler, server_type_t type, size_t workers) {
  http_server_t *srv = (http_server_t *)ualloc(sizeof(*srv));
  if(srv == NULL) {
    errno = ENOMEM;
    return NULL;
  }

  if(workers == 0)
    workers = sysconf(_SC_NPROCESSORS_ONLN) * 2;

  if((srv->pool = threadpool.init(workers)) == NULL)
    goto _error;

  if(pthread_mutex_init(srv->running_lock, NULL) < 0)
    goto _error;

  srv->handler = handler;
  srv->type = type;

  return srv;

  _error: {
    errno_t tmp = errno;
    pthread_mutex_unlock(srv->running_lock);
    pthread_mutex_destroy(srv->running_lock);
    threadpool.destroy(srv->pool);
    ufree(srv);
    errno = tmp;
    return NULL;
  }
}

static int http_server_add_bind(http_server_t *srv, sockaddr_t addr) {
  if(srv->running) {
    errno = ENOLCK;
    return -1;
  }

  struct pollfd fd = {
    .events = POLLIN
  };

  if((fd.fd = mksrvsock(SOCK_STREAM | SOCK_NONBLOCK, 0, &addr.data, addr.len)) < 0)
    goto _error;

  if(pollfd_list_add(srv->sockets, fd) < 0)
    goto _error;

  return 0;

  _error: {
    errno_t tmp = errno;
    close(fd.fd);
    errno = tmp;
    return -1;
  }
}

static int http_server_start(http_server_t *srv) {
  
  return 0;
}

static int http_server_stop(http_server_t *srv) {
  if(!check_running(srv))
    return -1;


  return 0;
}


static void http_server_destroy(http_server_t *srv) {
  if(srv == NULL)
    return;

  
}

const http_server_methods_t http_server = {
  http_server_init,
  http_server_add_bind,
  http_server_start,
  http_server_stop,
  http_server_destroy
};

