#pragma once

#include <stdint.h>
#include <sys/types.h>

long bytes2long (uint8_t *bytes, ssize_t n);
long bytes2long_from (uint8_t *bytes, ssize_t n, ssize_t from);

void long2bytes (long l, uint8_t *bytes, ssize_t n);
void long2bytes_to (long l, uint8_t *bytes, ssize_t n, ssize_t to);
