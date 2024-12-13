#include <unistd.h>

#include "utils/file_helpers.h"

ssize_t
cache_file_segment (int fd, int whence, uint8_t *buf, size_t count)
{
  lseek (fd, whence, SEEK_SET);
  return read (fd, buf, count);
}

ssize_t
flush_file_buf (int fd, int whence, const uint8_t *buf, size_t count)
{
  lseek (fd, whence, SEEK_SET);
  return write (fd, buf, count);
}
