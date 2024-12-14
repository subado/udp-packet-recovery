#include "globals.h"
#include "utils/log.h"
#include "utils/timespec_helpers.h"

#include "receive_handlers/recv_receive_timeout.h"

void
recv_receive_timeout ()
{
  size_t from = 0;
  deserialize_timespec (&timeout, received_packet.data, &from);

  print_verbose ("timeout[%ld %ld] was received\n", timeout.tv_sec,
                 timeout.tv_nsec);

  changing_remote_state = true;
  state = STATE_RECEIVE_PACKETS;
  desired_remote_state = STATE_SEND_PACKETS;
}
