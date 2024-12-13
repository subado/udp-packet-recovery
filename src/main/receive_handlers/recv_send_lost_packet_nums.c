#include "globals.h"
#include "receive_handlers/recv_receive_packets.h"
#include "utils/file_helpers.h"
#include "utils/log.h"

#include "receive_handlers/recv_send_lost_packet_nums.h"

void
recv_send_lost_packet_nums ()
{
  if ((received_packet.packet_num == packet_to_send.packet_num)
      && received_packet.packet_num != 0)
    {
      recv_receive_packets ();
    }

  if (received_packet_count == pending_packets_count)
    {
      changing_remote_state = true;
      desired_remote_state = STATE_SEND_PACKET_COUNT;
      state = STATE_RECEIVE_PACKET_COUNT | STATE_COMPLETE;

      flush_file_buf (fd, file_segment_idx * FILE_BUF_SIZE, file_buf,
                      pending_packets_size);

      print_verbose ("all lost packets were recovered\n");

      ++file_segment_idx;
    }
}
