#include "send_handlers/safe_send_packet.h"

#include "send_handlers/send_send_timeout.h"

void
send_send_timeout ()
{
  safe_send_packet ();
}
