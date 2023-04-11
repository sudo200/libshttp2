#ifndef ___SERVER_H__
#define ___SERVER_H__

#include "statuses.h"
#include "request.h"

#define NONNULL(...)  __attribute__((nonnull(__VA_ARGS__)))

typedef enum {
  SHTTP_HTTP,
} server_type_t;

typedef struct {
  int (*writeHead)(http_status_t status, const http_header_t *header);
  int (*write)(const void *buf, size_t sz);
  int (*writeBuf)(const buffer_t *buffer);
  int (*writeStr)(const char *str);
  __attribute__((format(printf, 1, 2))) int (*printf)(const char *format, ...);
} http_server_handler_methods_t;

typedef void (*http_server_handler_t)(const http_request_t *req, const http_server_handler_methods_t *res);
typedef struct http_server http_server_t;


http_server_t *http_server_new(http_server_handler_t handler, server_type_t type) NONNULL(1);

int http_server_add_bind(http_server_t *server, sockaddr_t addr, int backlog) NONNULL(1);

int http_server_start(http_server_t *server, param_cb_t cb, callback_param_t param) NONNULL(1);

int http_server_stop(http_server_t *server, param_cb_t cb, callback_param_t param) NONNULL(1);

void http_server_destroy(http_server_t *server);

#undef NONNULL

#endif //___SERVER_H__

