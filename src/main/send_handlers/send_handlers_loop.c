#include <time.h>

#include "globals.h"
#include "packet_preparators/generic_prepare_packet.h"
#include "send_handlers/safe_send_packet.h"
#include "send_handlers/send_receive_lost_packet_nums.h"
#include "send_handlers/send_send_lost_packet_nums.h"
#include "send_handlers/send_send_packet_count.h"
#include "send_handlers/send_send_packets.h"

#include "send_handlers/send_handlers_loop.h"

void
send_handlers_loop ()
{
  for (;;)
    {
      generic_prepare_packet ();

      if (changing_remote_state)
        {
          safe_send_packet ();
        }
      else
        {
          switch (state)
            {
            case STATE_SEND_PACKETS:
              send_send_packets ();
              break;

            case STATE_RECEIVE_LOST_PACKET_NUMS:
              send_receive_lost_packet_nums ();
              break;

            case STATE_SEND_LOST_PACKET_NUMS:
              send_send_lost_packet_nums ();
              break;

            case STATE_SEND_PACKET_COUNT:
              send_send_packet_count ();
              break;

            default:
              break;
            }
        }
      nanosleep (&req, &rem);
    }
}
