#include "globals.h"
#include "utils/log.h"
#include "utils/timespec_helpers.h"

#include "packet_preparators/prepare_send_timeout_packet.h"

void
prepare_send_timeout_packet ()
{
  packet_to_send.packet_num = 1;
  packet_to_send.state_num = STATE_RECEIVE_TIMEOUT;

  size_t to = 0;
  serialize_timespec (&timeout, packet_to_send.data, &to);

  print_verbose ("sending timeout[%ld %ld]\n", timeout.tv_sec,
                 timeout.tv_nsec);

  packet_to_send.size = to * 8;
}
