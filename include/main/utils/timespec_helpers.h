#pragma once

#include <stdint.h>
#include <time.h>

long double diff_timespec (const struct timespec *time_new,
                           const struct timespec *time_old);

void serialize_timespec (const struct timespec *time, uint8_t *buf,
                         size_t *to);
void deserialize_timespec (struct timespec *time, const uint8_t *buf,
                           size_t *from);
