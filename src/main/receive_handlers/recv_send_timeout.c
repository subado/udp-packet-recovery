#include <time.h>

#include "globals.h"

#include "receive_handlers/recv_send_timeout.h"

void
recv_send_timeout ()
{
  timespec_get (&end_timespec, TIME_UTC);
  timeout.tv_sec = (end_timespec.tv_sec - start_timespec.tv_sec);
  timeout.tv_nsec = (end_timespec.tv_nsec - start_timespec.tv_nsec);
  if (timeout.tv_nsec < 0)
    {
      --timeout.tv_sec;
      timeout.tv_nsec += 1000000000L;
    }
  timeout.tv_sec /= TIMEOUT_DIVISOR;
  timeout.tv_nsec /= TIMEOUT_DIVISOR;
}
