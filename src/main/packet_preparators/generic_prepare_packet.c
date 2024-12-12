#include "globals.h"
#include "packet_helpers.h"
#include "packet_preparators/prepare_send_lost_packet_nums_packet.h"
#include "packet_preparators/prepare_send_packet_count_packet.h"

#include "packet_preparators/generic_prepare_packet.h"

void
generic_prepare_packet ()
{
  static packet_state_t last_state = STATE_UNSPEC,
                        last_desired_remote_state = STATE_UNSPEC;
  static bool is_changed = false;

  if ((last_state != state) && !changing_remote_state)
    {
      is_changed = true;
      switch (state)
        {
        case STATE_SEND_PACKETS:
          packet_to_send.packet_num = 1;
          packet_to_send.state_num = STATE_RECEIVE_PACKETS;
          break;
        case STATE_RECEIVE_PACKETS:
          packet_to_send.packet_num = 1;
          break;
        case STATE_SEND_LOST_PACKET_NUMS:
          prepare_send_lost_packet_nums_packet ();
          break;
        case STATE_RECEIVE_LOST_PACKET_NUMS:
          packet_to_send.state_num = STATE_SEND_LOST_PACKET_NUMS;
          break;
        case STATE_SEND_PACKET_COUNT:
          prepare_send_packet_count_packet ();
          break;
        default:
          is_changed = false;
          break;
        }

      last_state = state;
    }

  if (changing_remote_state
      && (last_desired_remote_state != desired_remote_state))
    {
      packet_to_send.packet_num = 0;
      packet_to_send.size = 1;
      packet_to_send.state_num = desired_remote_state;
      is_changed = true;
      last_desired_remote_state = desired_remote_state;
    }

  if (is_changed)
    {
      serialize_packet (&packet_to_send, buf_to_send);
    }
}
