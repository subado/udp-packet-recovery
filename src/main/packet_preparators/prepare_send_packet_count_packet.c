#include <stdio.h>

#include "bytes2decimal.h"
#include "file_helpers.h"
#include "globals.h"

#include "packet_preparators/prepare_send_packet_count_packet.h"

void
prepare_send_packet_count_packet ()
{
  pending_packets_size = MIN (FILE_BUF_SIZE, file_remaining_size);

  if (pending_packets_size > 0)
    {
      pending_packets_count
          = pending_packets_size / PACKET_DATA_SIZE
            + ((pending_packets_size % PACKET_DATA_SIZE) > 0);
      file_remaining_size -= pending_packets_size;

      packet_to_send.packet_num = 1;
      packet_to_send.state_num = STATE_RECEIVE_PACKET_COUNT;
      packet_to_send.size = sizeof (pending_packets_count);
      decimal2bytes (pending_packets_count, packet_to_send.data,
                     packet_to_send.size);

      cache_file_segment (fd, FILE_BUF_SIZE * file_segment_idx, file_buf,
                          pending_packets_size);
#ifdef DEBUG
      printf ("sending file segment[%d] is started\n", file_segment_idx);
      printf ("sending packet count[%d]\n", pending_packets_count);
#endif
    }
  else
    {
      state = STATE_COMPLETE;
    }
}
