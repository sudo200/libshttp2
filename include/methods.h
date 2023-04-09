#ifndef ___METHODS_H___
#define ___METHODS_H___

#include <stddef.h>

typedef enum {
  HTTP_UNKNOWN_METHOD = -1,

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
  HTTP_UNSUBSCRIBE,
} http_method_t;

/**
 * @brief Returns a string representation of the given http method.
 * @param method  The method to 'stringify'.
 * @param len If not NULL, the length of the string will be written to the address pointed to by {@arg len}.
 * @returns The string representation of {@arg method}.
 */
const char *http_method_to_string(http_method_t method, size_t *len);

/**
 * @brief Returns the corresponding enum value for {@arg str}.
 * @param str The string to convert to an enum value.
 * @returns The corresponding enum value.
 */
http_method_t http_method_from_string(const char *str);

#endif  //___METHODS_H___

