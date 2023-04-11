#ifndef ___HEADER_H__
#define ___HEADER_H__

#include <sutil/types.h>

typedef struct http_header http_header_t;

#define NONNULL(...)  __attribute__((nonnull(__VA_ARGS__)))

/**
 * Creates a new, empty http header.
 */
http_header_t *http_header_new(void);

/**
 * Creates a http header from a NULL-terminated array of string pairs,
 * headers[][0] being the key and headers[][1] being the value.
 */
http_header_t *http_header_from_strings(const char *const *const headers[2]) NONNULL(1);

/**
 * Adds a new entry with given key and value.
 */
int http_header_add(http_header_t *header, const char *key, const char *value) NONNULL(1, 2);

/**
 * Parses the given header entry from an HTTP message
 * and adds the key and value to the header.
 */
int http_header_add_line(http_header_t *header, const char *line) NONNULL(1, 2);

/**
 * Returns true when the given key is found within the header.
 */
bool http_header_has(const http_header_t *header, const char *key) NONNULL(1, 2);

/**
 * Gets the value corresponding to key, or NULL if a key doesn't exist.
 */
const char *http_header_get(const http_header_t *header, const char *key) NONNULL(1, 2);

/**
 * Removes the entry identified by key from the header.
 */
int http_header_remove(http_header_t *header, const char *key) NONNULL(1, 2);

/**
 * Executes cb for each entry in the header.
 */
int http_header_foreach(const http_header_t *header, void (*cb)(const char *key, const char *value, void *pipe), void *pipe) NONNULL(1, 2);

/**
 * Returns a dynamically allocated, stringified, HTTP compatible header string.
 */
char *http_header_to_string(const http_header_t *header) NONNULL(1);

/**
 * Destroyes the header and all of its entries.
 */
void http_header_destroy(http_header_t *header);

#undef  NONNULL

#endif  //___HEADER_H__

