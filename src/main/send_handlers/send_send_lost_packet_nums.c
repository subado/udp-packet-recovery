#include "globals.h"
#include "packet_helpers.h"
#include "send_handlers/safe_send_packet.h"

#include "send_handlers/send_send_lost_packet_nums.h"

void
send_send_lost_packet_nums ()
{
  if (received_packet_count < pending_packets_count
      && received_packet_presence[packet_to_send.packet_num])
    {
      while (packet_to_send.packet_num < pending_packets_count
             && received_packet_presence[++packet_to_send.packet_num])
        ;
    }

  serialize_packet (&packet_to_send, buf_to_send);
  safe_send_packet ();
}
