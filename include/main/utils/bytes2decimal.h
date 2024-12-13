#pragma once

#include <stddef.h>
#include <stdint.h>

void bytes2decimal (uintmax_t *x, const uint8_t *bytes, size_t n);
void bytes2decimal_from (uintmax_t *x, const uint8_t *bytes, size_t n,
                         size_t from);

void decimal2bytes (uintmax_t x, uint8_t *bytes, size_t n);
void decimal2bytes_to (uintmax_t x, uint8_t *bytes, size_t n, size_t to);
