#include <arpa/inet.h>
#include <fcntl.h>
#include <getopt.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#include "getopt_loop.h"
#include "globals.h"
#include "init_globals.h"
#include "log.h"
#include "receive_handlers/receive_signal_handler.h"
#include "send_handlers/send_handlers_loop.h"
#include "socket_helpers.h"

int
main (int argc, char *argv[])
{
#ifdef DEBUG
  setbuf (stdout, NULL);
#endif

  init_globals ();

  const char *short_options = "cs4::6::p:r:f:";
  const struct option long_options[]
      = { { "client", no_argument, 0, 'c' },
          { "server", no_argument, 0, 's' },
          { "ipv4", optional_argument, 0, '4' },
          { "ipv6", optional_argument, 0, '6' },
          { "port", required_argument, 0, 'p' },
          { "remote_port", required_argument, 0, 'r' },
          { "filename", required_argument, 0, 'f' },
          { 0, 0, 0, 0 } };

  const char *required_options[] = { "cs", "46", "f" };
  getopt_loop (argc, argv, short_options, long_options,
               sizeof (required_options) / sizeof (required_options[0]),
               required_options);

  sfd = create_binded_socket (socket_family, &addr, socklen);
  printf ("socket is ready\n");
  printf ("socket is binded to port %d\n", port);

  bool ip_recverr_sockopt = true;
  setsockopt (sfd, IPPROTO_IP, IP_RECVERR, &ip_recverr_sockopt,
              sizeof (ip_recverr_sockopt));

  signal (SIGIO, receive_signal_handler);

  make_socket_async (sfd);

  fd = open (filename, open_flag, S_IRUSR | S_IWUSR);
  if (fd == -1)
    perror_exit ("cannot open an file");
  printf ("file[%s] is opened\n", filename);

  if (is_client)
    {
      struct stat fd_stat;
      if (fstat (fd, &fd_stat) == -1)
        {
          perror_exit ("cannot get a file status");
        }
      file_remaining_size = fd_stat.st_size;
      printf ("file size: %ld\n", file_remaining_size);

      if ((connect (sfd, (struct sockaddr *)&remote_addr, remote_socklen)
           == -1))
        {
          perror_exit ("cannot connect to remote server socket");
        }
      printf ("client is connected to remote server socket\n");
    }

  send_handlers_loop ();

  close (sfd);
  printf ("socket is closed\n");
  close (fd);
  printf ("file is closed\n");

  return 0;
}
