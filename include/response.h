#ifndef ___RESPONSE_H__
#define ___RESPONSE_H__

#include <sutil/buffer.h>
#include <sutil/network.h>

#include "header.h"
#include "statuses.h"
#include "version.h"

typedef struct {
  http_version_t version;
  http_status_t code;
  char *status_line;

  http_header_t *header;

  iobuffer_t *peer;
  sockaddr_t peer_addr;

  buffer_t *body;
} http_response_t;

#endif  //___RESPONSE_H__

