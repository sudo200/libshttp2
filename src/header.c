#include <errno.h>
#include <string.h>

#include <sutil/dmem.h>
#include <sutil/hash.h>
#include <sutil/hashmap.h>
#include <sutil/set.h>
#include <sutil/treeset.h>

#include "header.h"

#define stra(str) (void *)str, strlen((const char *)str)

typedef struct {
  void (*real_cb)(const char *, const char *, void *);
  void *real_pipe;
} helper_cb_data_t;

struct http_header {
  hashmap_t *map;
  set_t *refs;
};

// Private functions
static void ref_destr(void *ref, size_t sz __attribute__((unused)), void *pipe __attribute__((unused))) {
  ufree(ref);
}

static void helper_cb(void *key, size_t keysz __attribute__((unused)), void *value, void *pipe) {
  helper_cb_data_t *data = (helper_cb_data_t *)pipe;
  data->real_cb((const char *)key, (const char *)value, data->real_pipe);
}

// Public functions
http_header_t *http_header_new(void) {
  http_header_t *header = (http_header_t *)ualloc(sizeof(*header));
  if(header == NULL) {
    errno = ENOMEM;
    return NULL;
  }

  if((header->map = hashmap_new_prealloc(fnv1a, 5)) == NULL)
    return NULL;
  if((header->refs = (set_t *)treeset_new()) == NULL)
    return NULL;

  return header;
}

http_header_t *http_header_from_strings(const http_header_entry_t *entries) {
  http_header_t *header = http_header_new();
  if(header == NULL)
    return NULL;
  
  while (entries->key && entries->value) {
    if(http_header_add(header, entries->key, entries->value) < 0)
      goto _error;
    entries++;
  }

  return header;

  _error: {
    errno_t tmp = errno;
    http_header_destroy(header);
    errno = tmp;
    return NULL;
  }
}



bool http_header_has(const http_header_t *header, const char *key) {
  return hashmap_contains_key(header->map, stra(key));
}

const char *http_header_get(const http_header_t *header, const char *key) {
  return (const char *)hashmap_get(header->map, stra(key));
}

int http_header_remove(http_header_t *header, const char *key) {
  errno = 0;
  const char *value = (const char *)hashmap_remove(header->map, stra(key));
  if(value == NULL && errno != 0)
    return -1;

  set_remove(header->refs, stra(key));
  set_remove(header->refs, stra(value));
  //ufree(); TODO: somehow obtain key from hashmap
  ufree((void *)value);
  return 0;
}

int http_header_foreach(const http_header_t *header, void (*cb)(const char *, const char *, void *), void *pipe) {
  helper_cb_data_t data = {
    .real_cb = cb,
    .real_pipe = pipe,
  };
  return hashmap_foreach(header->map, helper_cb, &data);
}

void http_header_destroy(http_header_t *header) {
  if(header == NULL)
    return;

  set_foreach(header->refs, ref_destr, NULL);
  
  hashmap_destroy(header->map);
  set_destroy(header->refs);
  ufree(header);
}

#undef stra

