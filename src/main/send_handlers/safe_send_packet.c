#include <sys/socket.h>
#include <time.h>

#include "globals.h"

#include "send_handlers/safe_send_packet.h"

void
safe_send_packet ()
{
  while ((sent_n_bytes = sendto (
              sfd, &buf_to_send, PACKET_HEADER_SIZE + packet_to_send.size, 0,
              (struct sockaddr *)&remote_addr, remote_socklen))
         == -1)
    {
      nanosleep (&req, &rem);
    }
}
