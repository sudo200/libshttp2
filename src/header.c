#include <errno.h>
#include <string.h>

#include <sutil/dmem.h>
#include <sutil/hash.h>
#include <sutil/hashmap.h>
#include <sutil/mstring.h>

#include "header.h"

#define stra(str) (void *)str, strlen((const char *)str)

typedef struct {
  void (*real_cb)(const char *, const char *, void *);
  void *real_pipe;
} helper_cb_data_t;

struct http_header {
  hashmap_t *map;
};

// Private functions
static void ref_destr(void *key, size_t sz __attribute__((unused)), void *value, void *pipe __attribute__((unused))) {
  ufree(key);
  ufree(value);
}

static void helper_cb(void *key, size_t keysz __attribute__((unused)), void *value, void *pipe) {
  helper_cb_data_t *data = (helper_cb_data_t *)pipe;
  data->real_cb((const char *)key, (const char *)value, data->real_pipe);
}

static void to_string_cb(void *__key, size_t ___keysz __attribute__((unused)), void *__value, void *__pipe) {
  const char *key = (const char *)__key;
  const char *value = (const char *)__value;
  char **str = (char **)__pipe;

  char *line;
  msprintf(&line, "%s: %s\r\n", key, value);
  mstrcat(str, line);
  ufree(line);
}

// Public functions
static http_header_t *http_header_init(void) {
  http_header_t *header = (http_header_t *)ualloc(sizeof(*header));
  if(header == NULL) {
    errno = ENOMEM;
    return NULL;
  }

  if((header->map = hashmap_new_prealloc(fnv1a, 5)) == NULL)
    return NULL;

  return header;
}

static http_header_t *http_header_from_strings(const http_header_entry_t *entries) {
  http_header_t *header = http_header.init();
  if(header == NULL)
    return NULL;
  
  while (entries->key && entries->value) {
    if(http_header.add(header, entries->key, entries->value) < 0)
      goto _error;
    entries++;
  }

  return header;

  _error: {
    errno_t tmp = errno;
    http_header.destroy(header);
    errno = tmp;
    return NULL;
  }
}

static int http_header_add(http_header_t *header, const char *key, const char *value) {
  const size_t keysz = strlen(key), valuesz = strlen(value);
  char *_key = NULL, *_value = NULL;
 
  if((_key = ualloc(keysz + sizeof(*_key))) == NULL)
    goto _error;

  if((_value = ualloc(valuesz + sizeof(*_value))) == NULL)
    goto _error;

  memcpy(_key, key, keysz + sizeof(*_key));
  memcpy(_value, value, valuesz + sizeof(*_value));

  errno = 0;
  if(hashmap_put(header->map, _key, keysz, _value) == NULL && errno != 0)
    goto _error;

  return 0;

_error: {
    errno_t tmp = errno;
    ufree(_key);
    ufree(_value);
    errno = tmp;
    return -1;
  }
}

static int http_header_add_line(http_header_t *header, const char *__line) {
  char *line, *key = NULL, *value = NULL;
  if(msprintf(&line, "%s", __line) < 0)
    return -1;

  char *v = strchr(line, ':');
  if(v == NULL) {
    errno = EINVAL;
    goto _error;
  }

  *v++ = '\0';
  
  if(msprintf(&key, "%s", line) < 0)
    goto _error;
  if(msprintf(&value, "%s", v) < 0)
    goto _error;

  ufree(line);
  line = NULL;

  if(strtrim(&key) != 0)
    goto _error;
  if(strtrim(&value) != 0)
    goto _error;

  if(http_header.add(header, key, value) < 0)
    goto _error;

  ufree(key);
  ufree(value);

  return 0;

_error: {
    ufree(line);
    ufree(key);
    ufree(value);
    return -1;
  }
}

static bool http_header_has(const http_header_t *header, const char *key) {
  return hashmap_contains_key(header->map, stra(key));
}

const char *http_header_get(const http_header_t *header, const char *key) {
  return (const char *)hashmap_get(header->map, stra(key));
}

static int http_header_remove(http_header_t *header, const char *key) {
  errno = 0;
  void *value = hashmap_remove(header->map, stra(key));
  if(value == NULL && errno != 0)
    return -1;
  void *_key = hashmap_key(header->map, stra(key));

  ufree(_key);
  ufree(value);
  return 0;
}

static int http_header_foreach(const http_header_t *header, void (*cb)(const char *, const char *, void *), void *pipe) {
  helper_cb_data_t data = {
    .real_cb = cb,
    .real_pipe = pipe,
  };
  return hashmap_foreach(header->map, helper_cb, &data);
}

static char *http_header_to_string(const http_header_t *header) {
  char *str = NULL;
  if(hashmap_foreach(header->map, to_string_cb, &str) < 0) {
    ufree(str);
    return NULL;
  }
  return str;
}

static void http_header_destroy(http_header_t *header) {
  if(header == NULL)
    return;

  hashmap_foreach(header->map, ref_destr, NULL);
  hashmap_destroy(header->map);
  ufree(header);
}



const http_header_methods_t http_header = {
  http_header_init,
  http_header_from_strings,
  http_header_add,
  http_header_add_line,
  http_header_has,
  http_header_get,
  http_header_remove,
  http_header_foreach,
  http_header_to_string,
  http_header_destroy
};

#undef stra

