#include <time.h>

#include "globals.h"
#include "send_handlers/safe_send_packet.h"

#include "send_handlers/send_send_packet_count.h"

void
send_send_packet_count ()
{
  timespec_get (&start_timespec, TIME_UTC);
  safe_send_packet ();
}
