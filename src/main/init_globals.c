#include <string.h>

#include "globals.h"

#include "init_globals.h"

void
init_globals ()
{
  changing_remote_state = false;

  state = STATE_UNSPEC;

  socket_family = AF_UNSPEC;

  memset (&addr, 0, sizeof (addr));
  memset (&remote_addr, 0, sizeof (remote_addr));

  memset (&received_buf, 0, PACKET_SIZE);
  memset (&buf_to_send, 0, PACKET_SIZE);

  memset (&received_packet, 0, sizeof (received_packet));
  memset (&packet_to_send, 0, sizeof (packet_to_send));

  remote_port = port = PORT_NUM;

  filename = NULL;

  file_segment_idx = 0;
}
