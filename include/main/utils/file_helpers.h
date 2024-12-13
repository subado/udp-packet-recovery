#pragma once

#include <stdint.h>
#include <unistd.h>

ssize_t cache_file_segment (int fd, int whence, uint8_t *buf, size_t count);
ssize_t flush_file_buf (int fd, int whence, const uint8_t *buf, size_t count);
