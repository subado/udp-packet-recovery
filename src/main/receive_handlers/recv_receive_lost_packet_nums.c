#include "globals.h"
#include "packet_helpers.h"

#include "receive_handlers/recv_receive_lost_packet_nums.h"

void
recv_receive_lost_packet_nums ()
{
  if ((received_packet.packet_num != 0)
      && (received_packet.packet_num != packet_to_send.packet_num))
    {
      packet_to_send.packet_num = received_packet.packet_num;
      read_file_cache (&packet_to_send, file_buf);
    }
}
