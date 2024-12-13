#include <sys/time.h>

#include <linux/errqueue.h>
#include <stdio.h>

#include "config.h"
#include "globals.h"
#include "utils/log.h"
#include "utils/packet_helpers.h"

#include "receive_handlers/recv_receive_lost_packet_nums.h"
#include "receive_handlers/recv_receive_packet_count.h"
#include "receive_handlers/recv_receive_packets.h"
#include "receive_handlers/recv_send_lost_packet_nums.h"

#include "receive_handlers/receive_signal_handler.h"

void
receive_signal_handler (int signal)
{

  while (true)
    {
      if (changing_remote_state && (state & STATE_COMPLETE)
          && recvmsg (sfd, &message_header, MSG_ERRQUEUE) != -1)
        {
          struct cmsghdr *cmsg;               /* Control related data */
          struct sock_extended_err *sock_err; /* Struct describing the error */

          for (cmsg = CMSG_FIRSTHDR (&message_header); cmsg;
               cmsg = CMSG_NXTHDR (&message_header, cmsg))
            {
              /* Ip level and Error type */
              if (cmsg->cmsg_level == SOL_IP && cmsg->cmsg_type == IP_RECVERR)
                {
                  sock_err = (struct sock_extended_err *)CMSG_DATA (cmsg);
                  if (sock_err)
                    {
                      /* We are interested in ICMP errors */
                      if (sock_err->ee_origin == SO_EE_ORIGIN_ICMP
                          && sock_err->ee_type == ICMP_DEST_UNREACH)
                        {

                          print_verbose (
                              "ICMP destination unreachable error received\n");

                          changing_remote_state = false;
                          desired_remote_state = state = STATE_COMPLETE;
                        }
                    }
                }
            }
        }
      if (!is_client && remote_addr.ss_family == AF_UNSPEC)
        {
          recvfrom (sfd, &received_buf, PACKET_SIZE, 0,
                    (struct sockaddr *)&remote_addr, &remote_socklen);
          if (connect (sfd, (struct sockaddr *)&remote_addr, remote_socklen)
              == -1)
            {
              perror_exit ("cannot connect to remote client socket");
            }
          printf ("server is connected to remote client socket\n");
        }
      else if ((received_n_bytes = recv (sfd, &received_buf, PACKET_SIZE, 0))
               == -1)
        {

          break;
        }

      deserialize_packet (&received_packet, received_buf);

      if (!changing_remote_state && state != received_packet.state_num)
        {
          state = received_packet.state_num;
        }
      else if ((changing_remote_state && (received_packet.state_num & state)))
        {
          state = received_packet.state_num & state;
          changing_remote_state = false;
        }

      if (changing_remote_state)
        continue;

      switch (state)
        {
        case STATE_SEND_LOST_PACKET_NUMS:
          recv_send_lost_packet_nums ();
          break;
        case STATE_RECEIVE_PACKETS:
          recv_receive_packets ();
          break;
        case STATE_RECEIVE_LOST_PACKET_NUMS:
          recv_receive_lost_packet_nums ();
          break;
        case STATE_RECEIVE_PACKET_COUNT:
          recv_receive_packet_count ();
          break;
        default:
          break;
        }
    }
}
