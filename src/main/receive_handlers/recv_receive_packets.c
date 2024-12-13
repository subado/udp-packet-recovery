#include "globals.h"
#include "utils/log.h"
#include "utils/packet_helpers.h"

#include "receive_handlers/recv_receive_packets.h"

void
recv_receive_packets ()
{
  if (!received_packet_presence[received_packet.packet_num])
    {

      write_file_cache (&received_packet, file_buf);
      pending_packets_size += received_packet.size;
      ++received_packet_count;

      print_verbose ("packet[%d] was received (received packet count = %d)\n",
                     received_packet.packet_num, received_packet_count);

      received_packet_presence[received_packet.packet_num] = true;
    }
}
