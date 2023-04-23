#ifndef ___SHTTP2_SERVER_STRUCT_H___
#define ___SHTTP2_SERVER_STRUCT_H___

#include <pthread.h>

#include "pollfd_list.h"
#include "threadpool.h"
#include "server.h"

struct http_server {
  server_type_t type;
  http_server_handler_t handler;
  threadpool_t *pool;
  pthread_t conn_handler;
  pollfd_list_t sockets[1];

  pthread_mutex_t running_lock[1];
  bool running: 1;
};

#endif  //___SHTTP2_SERVER_STRUCT_H___

