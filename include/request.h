#ifndef ___REQUEST_H__
#define ___REQUEST_H__

#include <sutil/buffer.h>
#include <sutil/network.h>

#include "header.h"
#include "methods.h"
#include "version.h"

typedef struct {
  http_method_t method;
  char *url;
  http_version_t version;

  http_header_t *header;

  /** The other side of the connection
   */
  iobuffer_t *peer;

  /** The peer's address
   */
  sockaddr_t peer_addr;

  /**
   * The request body
   */
  buffer_t *body;
} http_request_t;

#endif  //___REQUEST_H__

