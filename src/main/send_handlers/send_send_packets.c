
#include <stdio.h>

#include "globals.h"
#include "packet_helpers.h"

#include "send_handlers/safe_send_packet.h"
#include "send_handlers/send_send_packets.h"

void
send_send_packets ()
{
  read_file_cache (&packet_to_send, file_buf);

  serialize_packet (&packet_to_send, buf_to_send);

  safe_send_packet ();

#ifdef DEBUG
  printf ("packet[%d] was sent\n", packet_to_send.packet_num);
#endif

  --pending_packets_count;

  if (pending_packets_count == 0)
    {
      changing_remote_state = true;
      state = STATE_RECEIVE_LOST_PACKET_NUMS | STATE_SEND_PACKET_COUNT;
      desired_remote_state = STATE_SEND_LOST_PACKET_NUMS;
#ifdef DEBUG
      printf ("sending file segment[%d] is "
              "completed\n",
              file_segment_idx);
#endif
      ++file_segment_idx;
    }
  else
    {
      ++packet_to_send.packet_num;
    }
}
