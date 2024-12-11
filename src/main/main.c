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
#include "receive_handler.h"
#include "send_loop.h"
#include "socket_helpers.h"

int
main (int argc, char *argv[])
{
  init_globals ();

  const char *short_options = "cs4:6:p:r:f:";
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

  sfd = create_binded_socket (socket_family, addr, socklen);
  printf ("socket is ready\n");
  printf ("socket is binded to port %d\n", port);

  signal (SIGIO, receive_handler);

  make_socket_async (sfd);

  fd = open (filename, open_flag);
  if (fd == -1)
    perror_exit ("cannot open an file");
  printf ("input file is opened\n");

  if (is_client)
    {
      struct stat fd_stat;
      if (fstat (fd, &fd_stat) == -1)
        {
          perror_exit ("cannot get a file status");
        }
      file_remaining_size = fd_stat.st_size;
      printf ("file size: %ld\n", file_remaining_size);
    }

  send_loop ();

  close (sfd);
  printf ("socket is closed\n");
  close (fd);
  printf ("file is closed\n");

  return 0;
}
