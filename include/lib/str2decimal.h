#pragma once

#include <stddef.h>
#include <stdint.h>

int str2long (uint64_t *out, const char *s, int base);
int str2int (uint32_t *out, const char *s, int base);
int str2short (uint16_t *out, const char *s, int base);

int str2long_range (uint64_t *out, const char *s, int base,
                    uint64_t min_value);
int str2int_range (uint32_t *out, const char *s, int base, uint32_t min_value);
int str2short_range (uint16_t *out, const char *s, int base,
                     uint16_t min_value);

int str2decimal (uintmax_t *out, const char *s, int base, size_t size);

int str2decimal_range (uintmax_t *out, const char *s, int base,
                       uintmax_t min_value, uintmax_t max_value, size_t size);
