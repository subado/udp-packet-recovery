
#include "globals.h"
#include "packet_helpers.h"
#include "send_handlers/safe_send_packet.h"

#include "send_handlers/send_receive_lost_packet_nums.h"

void
send_receive_lost_packet_nums ()
{
  if (packet_to_send.packet_num > 0)
    {
      read_file_cache (&packet_to_send, file_buf);
      serialize_packet (&packet_to_send, buf_to_send);
      safe_send_packet ();
    }
}
