#include <arpa/inet.h>
#include <fcntl.h>
#include <getopt.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include "getopt_loop.h"
#include "globals.h"
#include "init_globals.h"
#include "receive_handlers/receive_signal_handler.h"
#include "send_handlers/send_handlers_loop.h"
#include "utils/log.h"
#include "utils/socket_helpers.h"
#include "utils/timespec_helpers.h"

void free_resources (int signal);

int
main (int argc, char *argv[])
{
  setbuf (stdout, NULL);

  timespec_get (&start_timespec, TIME_UTC);
  srand (start_timespec.tv_nsec * getpid ());

  init_globals ();

  const char *short_options = "cs4::6::p:r:f:vmn:h";
  const struct option long_options[]
      = { { "client", no_argument, 0, 'c' },
          { "server", no_argument, 0, 's' },
          { "ipv4", optional_argument, 0, '4' },
          { "ipv6", optional_argument, 0, '6' },
          { "port", required_argument, 0, 'p' },
          { "remote_port", required_argument, 0, 'r' },
          { "filename", required_argument, 0, 'f' },
          { "verbose", no_argument, 0, 'v' },
          { "help", no_argument, 0, 'h' },
          /* client only options */
          { "measure_avg_speed", no_argument, 0, 'm' },
          { "skip_packets", required_argument, 0, 'n' },
          { 0, 0, 0, 0 } };

  const char *help_messages[] = {
    "run as client (send a file)",                      /* client */
    "run as server (receive a file)",                   /* server */
    "use ipv4 for socket addresses\n"                   /* ipv4 */
    "(client should pass server address as argument)",  /* ipv4 */
    "use ipv6 for socket addresses\n"                   /* ipv6 */
    "(client should pass server address as argument)",  /* ipv6 */
    "port to bind on the local socket",                 /* port */
    "port to connect on the remote host",               /* remote_port */
    "filename of an input file (client) or\n"           /* filename */
    "an output file (server)",                          /* filename */
    "explain what is being done",                       /* verbose */
    "display this help and exit",                       /* help */
    "(client only) measure the average sending speed\n" /* measure_avg_speed */
    "print it after sending a file is completed",       /* measure_avg_speed */
    "(client only) skip random {arg} packets",          /* skip_packets */
  };

  const char *required_options[] = { "cs", "46", "f" };
  getopt_loop (argc, argv, short_options, long_options,
               sizeof (required_options) / sizeof (required_options[0]),
               required_options, help_messages);

  sfd = create_binded_socket (socket_family, &addr, socklen);
  printf ("socket is binded to port %d\n", port);

  bool ip_recverr_sockopt = true;
  setsockopt (sfd, IPPROTO_IP, IP_RECVERR, &ip_recverr_sockopt,
              sizeof (ip_recverr_sockopt));

  signal (SIGIO, receive_signal_handler);

  make_socket_async (sfd);

  fd = open (filename, open_flag, S_IRUSR | S_IWUSR);
  if (fd == -1)
    perror_exit ("cannot open an file");
  print_verbose ("file[%s] is opened\n", filename);

  signal (SIGINT, free_resources);

  struct stat fd_stat;
  struct timespec sending_start_timespec;
  if (is_client)
    {
      if (fstat (fd, &fd_stat) == -1)
        {
          perror_exit ("cannot get a file status");
        }
      file_remaining_size = fd_stat.st_size;
      print_verbose ("file size: %ld\n", file_remaining_size);

      if ((connect (sfd, (struct sockaddr *)&remote_addr, remote_socklen)
           == -1))
        {
          perror_exit ("cannot connect to remote server socket");
        }
      printf ("client is connected to remote server socket\n");

      if (measure_avg_speed)
        {
          timespec_get (&sending_start_timespec, TIME_UTC);
        }

      if (n_skip_packets)
        {
          skip_packets_rate = (fd_stat.st_size / PACKET_DATA_SIZE
                               + (fd_stat.st_size % PACKET_DATA_SIZE > 0))
                              / n_skip_packets;
        }
    }

  send_handlers_loop ();

  if (is_client && measure_avg_speed)
    {
      struct timespec sending_end_timespec;
      timespec_get (&sending_end_timespec, TIME_UTC);
      long double speed = ((long double)(fd_stat.st_size) / 1024
                           / (diff_timespec (&sending_end_timespec,
                                             &sending_start_timespec)));
      printf ("average sending speed: %Lf KB/S\n", speed);
    }

  free_resources (0);

  return 0;
}

void
free_resources (int signal)
{
  close (sfd);
  print_verbose ("socket is closed\n");
  close (fd);
  print_verbose ("file is closed\n");
  exit (0);
}
