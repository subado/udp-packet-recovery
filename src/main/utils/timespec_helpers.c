#include "utils/timespec_helpers.h"
#include "utils/packet_helpers.h"

long double
diff_timespec (const struct timespec *time_new,
               const struct timespec *time_old)
{
  return (time_new->tv_sec - time_old->tv_sec)
         + (time_new->tv_nsec - time_old->tv_nsec) / 1000000000.0L;
}

void
serialize_timespec (const struct timespec *time, uint8_t *buf, size_t *to)
{
  serialize_decimal (&time->tv_sec, buf, sizeof (time->tv_sec), to);
  serialize_decimal (&time->tv_nsec, buf, sizeof (time->tv_nsec), to);
}

void
deserialize_timespec (struct timespec *time, const uint8_t *buf, size_t *from)
{
  deserialize_decimal (&time->tv_sec, buf, sizeof (time->tv_sec), from);
  deserialize_decimal (&time->tv_nsec, buf, sizeof (time->tv_nsec), from);
}
