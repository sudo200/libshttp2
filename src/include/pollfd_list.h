#ifndef ___POLLFD_LIST_H__
#define ___POLLFD_LIST_H__

#include <poll.h>

#include <sutil/types.h>

#define NONNULL(...)  __attribute__((nonnull(__VA_ARGS__)))

typedef struct {
  struct pollfd *fds;
  size_t nfds;
} pollfd_list_t;

int pollfd_list_add(pollfd_list_t *list, struct pollfd fd) NONNULL(1);

#undef NONNULL

#endif  //___POLLFD_LIST_H__

