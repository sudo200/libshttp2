#include <errno.h>

#include <sutil/dmem.h>

#include "pollfd_list.h"

int pollfd_list_add(pollfd_list_t *list, struct pollfd fd) {
  if((list->fds = urealloc(list->fds,
          (list->nfds + 1) * sizeof(*list->fds))) == NULL)
    return -1;

  list->fds[list->nfds++] = fd;

  return 0;
}

