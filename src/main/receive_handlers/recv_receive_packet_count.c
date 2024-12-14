#include <string.h>

#include "globals.h"
#include "utils/bytes2decimal.h"
#include "utils/log.h"

#include "receive_handlers/recv_receive_packet_count.h"

void
recv_receive_packet_count ()
{
  pending_packets_size = 0;
  received_packet_count = 0;

  bytes2decimal ((uintmax_t *)&pending_packets_count, received_packet.data,
                 received_packet.size);

  pending_packets_count = ntohl (pending_packets_count);
  print_verbose ("packet count[%d] was received\n", pending_packets_count);

  if (pending_packets_count > 0)
    {

      memset (received_packet_presence, 0, RECEIVED_PACKET_PRESENCE_SIZE);

      changing_remote_state = true;
      state = STATE_RECEIVE_TIMEOUT;
      desired_remote_state = STATE_SEND_TIMEOUT;
    }
}
