#include <string.h>
#include <time.h>

#include "bytes2long.h"
#include "file_helpers.h"
#include "globals.h"

#include "send_loop.h"

void
send_loop ()
{
  for (;;)
    {
      prepare_data ();

      switch (state)
        {
        case STATE_SENDING_PACKETS:
          {
            packet_to_send.size = MIN (PACKET_DATA_SIZE, pending_packets_size);

            for (size_t i = 0; i < packet_to_send.size; ++i)
              {
                packet_to_send.data[i]
                    = file_buf[i
                               + (packet_to_send.packet_num - 1)
                                     * PACKET_DATA_SIZE];
              }

            send_packet ();

            ++packet_to_send.size;

            if (packet_to_send.size > pending_packets_count)
              {
                state = STATE_CHANGING_REMOTE_STATE;
                desired_remote_state = STATE_SENDING_LOST_PACKET_NUMS;
                next_state = STATE_RECEIVING_LOST_PACKET_NUMS;
              }
            else
              {
                pending_packets_size -= packet_to_send.size;
              }

            break;
          }
        case STATE_SENDING_LOST_PACKET_NUMS:
          {
            if (received_packet_presence[packet_to_send.packet_num])
              {
                PACKET_NUM_TYPE i;
                for (i = packet_to_send.packet_num; i <= pending_packets_count;
                     ++i)
                  {
                    if (!received_packet_presence[i])
                      {
                        packet_to_send.packet_num = i;
                        break;
                      }
                  }
                if (i > pending_packets_count)
                  {
                    state = STATE_CHANGING_REMOTE_STATE;
                    desired_remote_state = STATE_SENDING_PACKET_COUNT;
                    next_state = STATE_RECEIVING_PACKET_COUNT;
                    continue;
                  }
              }
            send_packet ();
            break;
          }
        case STATE_CHANGING_REMOTE_STATE:
        case STATE_SENDING_PACKET_COUNT:
          {
            send_packet ();
            break;
          }
        }
      nanosleep (&req, &rem);
    }
}

void
prepare_data ()
{
  static state_t last_state = STATE_UNSPEC;
  if (last_state != state)
    {
      switch (state)
        {
        case STATE_SENDING_PACKETS:
          {
            packet_to_send.packet_num = 1;
            packet_to_send.state_num = STATE_RECEIVING_PACKETS;
            break;
          }
        case STATE_SENDING_LOST_PACKET_NUMS:
          {
            packet_to_send.packet_num = 1;
            packet_to_send.state_num = STATE_RECEIVING_LOST_PACKET_NUMS;
            break;
          }
        case STATE_SENDING_PACKET_COUNT:
          {
            pending_packets_size = MIN (FILE_BUF_SIZE, file_remaining_size);
            pending_packets_count
                = pending_packets_size / PACKET_DATA_SIZE
                  + ((pending_packets_size % PACKET_DATA_SIZE) > 0);
            file_remaining_size -= pending_packets_size;

            packet_to_send.packet_num = 1;
            packet_to_send.state_num = STATE_RECEIVING_PACKET_COUNT;
            packet_to_send.size = sizeof (pending_packets_count);
            long2bytes (pending_packets_count, packet_to_send.data,
                        packet_to_send.size);

            cache_file_segment (fd, FILE_BUF_SIZE * file_segment_idx, file_buf,
                                pending_packets_size);
            break;
          }
        case STATE_CHANGING_REMOTE_STATE:
          {
            packet_to_send.state_num = desired_remote_state;
            break;
          }
        }
      last_state = state;
    }
}

void
send_packet ()
{
  sent_n_bytes
      = sendto (sfd, (const void *)&packet_to_send, packet_to_send.size, 0,
                (struct sockaddr *)remote_addr, remote_socklen);
}
