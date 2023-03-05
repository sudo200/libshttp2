#include <errno.h>
#include <string.h>

#include <sys/socket.h>

#include <sutil/dmem.h>

#include "iobuffer.h"

struct iobuffer {
  int sendflags;
  int recvflags;

  int fd;
  FILE *stream;
  uint8_t buffer[IOBUFFER_SIZE];
};

iobuffer_t *iobuffer_new(int fd, int sendflags, int recvflags) {
  iobuffer_t *io = (iobuffer_t *) ualloc(sizeof(iobuffer_t));

  if(io == NULL) {
    errno = ENOMEM;
    return NULL;
  }

  if((io->stream = fmemopen(io->buffer, sizeof(io->buffer), "w+")) == NULL)
    return NULL;

  io->sendflags = sendflags;
  io->recvflags = recvflags;
  io->fd = fd;

  return io;
}

void iobuffer_destroy(iobuffer_t *io) {
  if(io == NULL)
    return;

  fclose(io->stream);
  ufree(io);
}

void iobuffer_clear(iobuffer_t *io) {
  memset(io->buffer, 0, sizeof(io->buffer));
}

int iobuffer_send(iobuffer_t *io, int flags) {
  size_t to_send = ftell(io->stream),
         sent = 0;

  while(to_send > sent) {
    ssize_t ret = send(
        io->fd,
        io->buffer + sent,
        to_send - sent,
        io->sendflags ^ flags
        );
    if(ret < 0)
      return -1;
    sent += ret;
  }

  fseek(io->stream, 0, SEEK_SET);
  return sent;
}

int iobuffer_recv(iobuffer_t *io, int flags) {
  fseek(io->stream, 0, SEEK_SET);
  return recv(io->fd, io->buffer, sizeof(io->buffer), io->recvflags ^ flags);
}

