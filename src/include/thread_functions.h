#ifndef ___SHTTP2_THREAD_FUNCTIONS___
#define ___SHTTP2_THREAD_FUNCTIONS___

#include <errno.h>

#include <sutil/dmem.h>

#include "server_struct.h"
#include "server.h"

typedef struct {
  sockaddr_t addr;
  fd_t fd;
} client_arg_t;

static void client_task(threadpool_task_arg_t arg) {
  client_arg_t *client = (client_arg_t *)arg.ptr;


  ufree(client);
}

static void accept_connection(threadpool_t *pool, fd_t conn) {
  client_arg_t *client = (client_arg_t *)ualloc(sizeof(*client));
  client->addr.len = sizeof(client->addr.data);
  client->fd = accept(conn, &client->addr.data, &client->addr.len);
  if(client->fd < 0) {
    if(errno != EAGAIN) {
      // Handle Error
    }
    ufree(client);
    return;
  }

  threadpool.submit(pool, (threadpool_task_t) {
      .func = client_task,
      .arg = { .ptr = client }
  });
}

static void *connection_worker(void *___ptr) {
  http_server_t *srv = (http_server_t *)___ptr;

  while(true) {
    int ret = poll(srv->sockets->fds, srv->sockets->nfds, -1);
    if(ret == 0)
      continue;

    if(ret < 0)
    {
      // Error
      continue;
    }

    for(size_t i = 0; i < srv->sockets->nfds; i++) {
      if(srv->sockets->fds[i].revents & POLLIN)
        accept_connection(srv->pool, srv->sockets->fds[i].fd);
    }
  }
}

#endif  //___SHTTP2_THREAD_FUNCTIONS___

