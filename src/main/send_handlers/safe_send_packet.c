#include <sys/socket.h>
#include <time.h>

#include "globals.h"

#include "send_handlers/safe_send_packet.h"

void
safe_send_packet ()
{
  for (size_t retries = 0;
       ((sent_n_bytes = send (sfd, &buf_to_send,
                              PACKET_HEADER_SIZE + packet_to_send.size, 0))
        == -1)
       && retries < MAX_RETRIES;
       ++retries)
    {
      nanosleep (&timeout, &timeout_rem);
    }
}
