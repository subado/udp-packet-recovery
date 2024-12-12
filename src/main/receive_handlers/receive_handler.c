#include <stddef.h>

#include "config.h"
#include "globals.h"
#include "packet_helpers.h"

#include "receive_handlers/recv_receive_lost_packet_nums.h"
#include "receive_handlers/recv_receive_packet_count.h"
#include "receive_handlers/recv_receive_packets.h"
#include "receive_handlers/recv_send_lost_packet_nums.h"

#include "receive_handlers/receive_signal_handler.h"

void
receive_signal_handler (int signal)
{
  while ((received_n_bytes
          = recvfrom (sfd, &received_buf, PACKET_SIZE, 0,
                      (struct sockaddr *)&remote_addr, &remote_socklen))
         != -1)
    {
      deserialize_packet (&received_packet, received_buf);

      if (!changing_remote_state && state != received_packet.state_num)
        {
          state = received_packet.state_num;
        }
      else if ((changing_remote_state && (received_packet.state_num & state)))
        {
          state = received_packet.state_num & state;
          changing_remote_state = false;
        }

      if (changing_remote_state)
        continue;

      switch (state)
        {
        case STATE_SEND_LOST_PACKET_NUMS:
          recv_send_lost_packet_nums ();
          break;
        case STATE_RECEIVE_PACKETS:
          recv_receive_packets ();
          break;
        case STATE_RECEIVE_LOST_PACKET_NUMS:
          recv_receive_lost_packet_nums ();
          break;
        case STATE_RECEIVE_PACKET_COUNT:
          recv_receive_packet_count ();
          break;
        case STATE_COMPLETE:
          {
            changing_remote_state = true;
            desired_remote_state = STATE_COMPLETE;
            state = STATE_COMPLETE;
            break;
          }
        default:
          break;
        }
    }
}
