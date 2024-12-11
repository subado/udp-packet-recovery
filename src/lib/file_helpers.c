#include "file_helpers.h"
#include <unistd.h>

ssize_t
cache_file_segment (int fd, int whence, uint8_t *buf, size_t count)
{
  lseek (fd, SEEK_SET, whence);
  return read (fd, buf, count);
}

ssize_t
flush_file_buf (int fd, int whence, const uint8_t *buf, size_t count)
{
  lseek (fd, SEEK_SET, whence);
  return write (fd, buf, count);
}
