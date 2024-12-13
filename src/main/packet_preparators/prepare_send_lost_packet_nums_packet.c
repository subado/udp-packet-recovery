#include "globals.h"
#include "utils/log.h"

#include "packet_preparators/prepare_send_lost_packet_nums_packet.h"

void
prepare_send_lost_packet_nums_packet ()
{
  print_verbose ("start recovering %d lost packets\n",
                 pending_packets_count - received_packet_count);
  packet_to_send.packet_num = 1;
  packet_to_send.state_num = STATE_RECEIVE_LOST_PACKET_NUMS;
}
