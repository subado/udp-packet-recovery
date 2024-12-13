#include <stdlib.h>

#include "globals.h"
#include "utils/log.h"
#include "utils/packet_helpers.h"

#include "send_handlers/safe_send_packet.h"
#include "send_handlers/send_send_packets.h"

void
send_send_packets ()
{
  static bool skip_packet = false;

  if (is_client && n_skip_packets > 0)
    {
      if (skip_packets_rate > 0
          && (packet_to_send.packet_num - last_skipped_packet)
                 >= skip_packets_rate)
        {
          skip_packet = true;
        }
      else
        {
          skip_packet = rand () % 2;
        }
    }

  if (skip_packet)
    {
      last_skipped_packet = file_segment_idx * PACKET_DATA_IN_FILE_BUF_COUNT
                            + packet_to_send.packet_num;
      --n_skip_packets;
      skip_packet = false;
    }
  else
    {
      read_file_cache (&packet_to_send, file_buf);

      serialize_packet (&packet_to_send, buf_to_send);

      safe_send_packet ();
      print_verbose ("packet[%d] was sent\n", packet_to_send.packet_num);
    }

  --pending_packets_count;

  if (pending_packets_count == 0)
    {
      changing_remote_state = true;
      state = STATE_RECEIVE_LOST_PACKET_NUMS | STATE_SEND_PACKET_COUNT;
      desired_remote_state = STATE_SEND_LOST_PACKET_NUMS;
      print_verbose ("sending file segment[%d] is "
                     "completed\n",
                     file_segment_idx);
      ++file_segment_idx;
    }
  else
    {
      ++packet_to_send.packet_num;
    }
}
