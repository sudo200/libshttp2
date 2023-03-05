#ifndef ___IOBUFFER_H__
#define ___IOBUFFER_H__

#ifndef IOBUFFER_SIZE
#define IOBUFFER_SIZE 4096
#endif

#include <stdio.h>

typedef struct iobuffer iobuffer_t;

/**
 * Wrap the file descriptor fd in a new iobuffer.
 * @param fd  The file descriptor to wrap.
 * @return  A pointer to the new iobuffer.
 */
iobuffer_t *iobuffer_new(int fd, int sendflags, int recvflags);

FILE *iobuffer_stream(iobuffer_t *iobuffer);

int iobuffer_send(iobuffer_t *iobuffer, int flags);

int iobuffer_recv(iobuffer_t *iobuffer, int flags);

void iobuffer_clear(iobuffer_t *iobuffer);

void iobuffer_destroy(iobuffer_t *iobuffer);

#endif  //___IOBUFFER_H__

