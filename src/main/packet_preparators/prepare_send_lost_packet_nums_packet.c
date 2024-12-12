#include <stdio.h>

#include "globals.h"

#include "packet_preparators/prepare_send_lost_packet_nums_packet.h"

void
prepare_send_lost_packet_nums_packet ()
{
#ifdef DEBUG
  printf ("start recovering %d lost packets\n",
          pending_packets_count - received_packet_count);
#endif
  packet_to_send.packet_num = 1;
  packet_to_send.state_num = STATE_RECEIVE_LOST_PACKET_NUMS;
}
