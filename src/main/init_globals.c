#include <string.h>

#include "globals.h"

#include "init_globals.h"

void
init_globals ()
{
  measure_avg_speed = verbose = false;
  changing_remote_state = false;
  last_skipped_packet = n_skip_packets = 0;

  timeout.tv_sec = DEFAULT_TIMEOUT_SEC;
  timeout.tv_nsec = DEFAULT_TIMEOUT_NSEC;

  state = STATE_UNSPEC;

  socket_family = AF_UNSPEC;

  memset (&addr, 0, sizeof (addr));
  memset (&remote_addr, 0, sizeof (remote_addr));

  memset (&icmp_msg_buf, 0, sizeof (icmp_msg_buf));

  memset (&received_buf, 0, sizeof (received_buf));
  memset (&buf_to_send, 0, sizeof (buf_to_send));

  memset (&received_packet, 0, sizeof (received_packet));
  memset (&packet_to_send, 0, sizeof (packet_to_send));

  remote_port = port = PORT_NUM;

  filename = NULL;

  file_segment_idx = 0;
}

void
init_message_header ()
{
  message_header_iov.iov_base = &icmp_header;
  message_header_iov.iov_len = sizeof (icmp_header);
  message_header.msg_name = (void *)&remote_addr;
  message_header.msg_namelen = remote_socklen;
  message_header.msg_iov = &message_header_iov;
  message_header.msg_iovlen = 1;
  message_header.msg_flags = 0;
  message_header.msg_control = icmp_msg_buf;
  message_header.msg_controllen = sizeof (icmp_msg_buf);
}
