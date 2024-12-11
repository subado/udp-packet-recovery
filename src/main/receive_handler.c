#include <stddef.h>
#include <string.h>

#include "bytes2long.h"
#include "config.h"
#include "globals.h"

#include "receive_handler.h"

void
receive_handler (int signal)
{
  while ((received_n_bytes
          = recvfrom (sfd, (void *)&received_packet, PACKET_SIZE, 0, NULL, 0))
         != -1)
    {
      if ((state != STATE_CHANGING_REMOTE_STATE
           && state != received_packet.state_num)
          || (state == STATE_CHANGING_REMOTE_STATE
              && received_packet.state_num == next_state))
        {
          state = received_packet.state_num;
        }

      switch (state)
        {

        case STATE_RECEIVING_PACKETS:
          {
            for (size_t i = 0; i < received_packet.size; ++i)
              {
                file_buf[PACKET_DATA_SIZE * (received_packet.packet_num - 1)]
                    = received_packet.data[i];
              }

            received_packet_presence[received_packet.packet_num] = true;
            break;
          }

        case STATE_RECEIVING_LOST_PACKET_NUMS:
          {
            break;
          }
        case STATE_RECEIVING_PACKET_COUNT:
          {
            memset (received_packet_presence, 0,
                    PACKET_DATA_IN_FILE_BUF_COUNT);

            pending_packets_count
                = bytes2long (received_packet.data, received_packet.size);
            state = STATE_CHANGING_REMOTE_STATE;
            next_state = STATE_RECEIVING_PACKETS;
            desired_remote_state = STATE_SENDING_PACKETS;
            break;
          }
        }
    }
}
