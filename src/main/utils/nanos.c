#include <stdint.h>
#include <time.h>

#include "utils/nanos.h"

uintmax_t
get_nanos (void)
{
  struct timespec ts;
  timespec_get (&ts, TIME_UTC);
  return (uintmax_t)ts.tv_sec * 1000000000UL + ts.tv_nsec;
}
