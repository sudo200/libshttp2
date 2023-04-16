#ifndef ___HEADER_H__
#define ___HEADER_H__

#include <sutil/types.h>

typedef struct http_header http_header_t;
typedef struct {
  const char *key;
  const char *value;
} http_header_entry_t;

#define NONNULL(...)  __attribute__((nonnull(__VA_ARGS__)))

typedef struct {

/**
 * Creates a new, empty http header.
 */
http_header_t *(*init)(void);

/**
 * Creates a http header from a NULL-terminated array of string pairs,
 * headers[][0] being the key and headers[][1] being the value.
 */
http_header_t *(*from_strings)(const http_header_entry_t *entries) NONNULL(1);

/**
 * Adds a new entry with given key and value.
 */
int (*add)(http_header_t *header, const char *key, const char *value) NONNULL(1, 2);

/**
 * Parses the given header entry from an HTTP message
 * and adds the key and value to the header.
 */
int (*add_line)(http_header_t *header, const char *line) NONNULL(1, 2);

/**
 * Returns true when the given key is found within the header.
 */
bool (*has)(const http_header_t *header, const char *key) NONNULL(1, 2);

/**
 * Gets the value corresponding to key, or NULL if a key doesn't exist.
 */
const char *(*get)(const http_header_t *header, const char *key) NONNULL(1, 2);

/**
 * Removes the entry identified by key from the header.
 */
int (*remove)(http_header_t *header, const char *key) NONNULL(1, 2);

/**
 * Executes cb for each entry in the header.
 */
int (*foreach)(const http_header_t *header, void (*cb)(const char *key, const char *value, void *pipe), void *pipe) NONNULL(1, 2);

/**
 * Returns a dynamically allocated, stringified, HTTP compatible header string.
 */
char *(*to_string)(const http_header_t *header) NONNULL(1);

/**
 * Destroyes the header and all of its entries.
 */
void (*destroy)(http_header_t *header);

} http_header_methods_t;

extern const http_header_methods_t http_header;

#undef  NONNULL

#endif  //___HEADER_H__

