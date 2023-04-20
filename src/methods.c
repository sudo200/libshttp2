#include <stdlib.h>
#include <string.h>

#include <sutil/hash.h>
#include <sutil/hashmap.h>

#include "methods.h"

#define len(arr)  (sizeof(arr)/sizeof(*arr))

typedef struct {
  char *ptr;
  size_t sz;
} ___str_t;

static hashmap_t *method_map = NULL;

static const http_method_t methods[] = {
  HTTP_ACL,
  HTTP_BIND,
  HTTP_CHECKOUT,
  HTTP_CONNECT,
  HTTP_COPY,
  HTTP_DELETE,
  HTTP_GET,
  HTTP_HEAD,
  HTTP_LINK,
  HTTP_LOCK,
  HTTP_M_SEARCH,
  HTTP_MERGE,
  HTTP_MKACTIVITY,
  HTTP_MKCALENDAR,
  HTTP_MKCOL,
  HTTP_MOVE,
  HTTP_NOTIFY,
  HTTP_OPTIONS,
  HTTP_PATCH,
  HTTP_POST,
  HTTP_PROPFIND,
  HTTP_PROPPATCH,
  HTTP_PURGE,
  HTTP_PUT,
  HTTP_REBIND,
  HTTP_REPORT,
  HTTP_SEARCH,
  HTTP_SOURCE,
  HTTP_SUBSCRIBE,
  HTTP_TRACE,
  HTTP_UNBIND,
  HTTP_UNLINK,
  HTTP_UNLOCK,
  HTTP_UNSUBSCRIBE
};

static const ___str_t method_strs[] = {
  {"ACL", 3},
  {"BIND", 4},
  {"CHECKOUT", 8},
  {"CONNECT", 7},
  {"COPY", 4},
  {"DELETE", 6},
  {"GET", 3},
  {"HEAD", 4},
  {"LINK", 4},
  {"LOCK", 4},
  {"M-SEARCH", 8},
  {"MERGE", 5},
  {"MKACTIVITY", 9},
  {"MKCALENDAR",9},
  {"MKCOL",5},
  {"MOVE", 4},
  {"NOTIFY", 6},
  {"OPTIONS", 7},
  {"PATCH", 5},
  {"POST", 4},
  {"PROPFIND", 8},
  {"PROPPATCH", 9},
  {"PURGE", 5},
  {"PUT", 3},
  {"REBIND", 6},
  {"REPORT", 6},
  {"SEARCH", 6},
  {"SOURCE", 6},
  {"SUBSCRIBE", 9},
  {"TRACE", 5},
  {"UNBIND", 6},
  {"UNLINK", 6},
  {"UNLOCK", 6},
  {"UNSUBSCRIBE", 11},
};

static inline const char *from_string_helper(___str_t str, size_t *len) {
  if(len != NULL)
    *len = str.sz;
  return str.ptr;
}

const char *http_method_to_string(http_method_t method, size_t *len) {
  if(method == HTTP_UNKNOWN_METHOD)
    return NULL;
  return from_string_helper(method_strs[(size_t)method], len);
}

http_method_t http_method_from_string(const void *str, size_t len) {
  const http_method_t *method = (http_method_t *)hashmap_get(method_map,
      str, len);

  if(method == NULL)
    return HTTP_UNKNOWN_METHOD;
  return *method;
}


static __attribute__((constructor)) void c(void) {
  if((method_map = hashmap_new(fnv1a)) == NULL)
    abort();

  for(size_t i = 0; i < len(methods); i++)
    hashmap_put(method_map,
        (void *)method_strs[i].ptr,
        method_strs[i].sz,
        (void *)(methods + i));
}

static __attribute__((destructor)) void d(void) {
  hashmap_destroy(method_map);
}

