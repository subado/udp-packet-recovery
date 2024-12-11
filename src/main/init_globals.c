#include <string.h>

#include "globals.h"

#include "init_globals.h"

void
init_globals ()
{
  state = STATE_UNSPEC;

  socket_family = AF_UNSPEC;

  memset (&remote_addr_in, 0, sizeof (struct sockaddr_in));
  memset (&remote_addr_in6, 0, sizeof (struct sockaddr_in6));
  memset (&addr_in, 0, sizeof (struct sockaddr_in));
  memset (&addr_in6, 0, sizeof (struct sockaddr_in6));

  memset (&received_packet, 0, PACKET_SIZE);
  memset (&packet_to_send, 0, PACKET_SIZE);

  remote_port = port = PORT_NUM;

  filename = NULL;
}
