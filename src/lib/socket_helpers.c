#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "log.h"
#include "socket_helpers.h"
#include "str2decimal.h"

void
init_sockaddr_storage_in (struct sockaddr_storage *addr, socklen_t *socklen,
                          in_port_t port)
{
  struct sockaddr_in addr_in;
  struct in_addr sin_addr = { .s_addr = INADDR_ANY };
  addr_in.sin_family = AF_INET;
  addr_in.sin_port = htons (port);
  addr_in.sin_addr = sin_addr;
  (*socklen) = sizeof (struct sockaddr_in);
  memcpy (addr, &addr_in, *socklen);
}

void
init_sockaddr_storage_in6 (struct sockaddr_storage *addr, socklen_t *socklen,
                           in_port_t port)
{
  struct sockaddr_in6 addr_in6;
  addr_in6.sin6_family = AF_INET6;
  addr_in6.sin6_port = htons (port);
  addr_in6.sin6_addr = in6addr_any;
  (*socklen) = sizeof (struct sockaddr_in6);

  memcpy (addr, &addr_in6, *socklen);
}

void
str2port (const char *s, in_port_t *p)
{
  if ((errno = str2short_range (p, s, 10, 1)) != 0)
    {
      error_exit ("%s is not a valid server port number\n", s);
    }
}

void
print_recv_info (uint8_t *msg, ssize_t n_bytes, int socket_family,
                 const struct sockaddr_storage *sender_addr,
                 char *sender_addr_str, const size_t sender_addr_str_len)
{
  const void *sender_addr_real_addr = NULL;
  in_port_t sender_addr_real_port;

  if (socket_family == AF_INET)
    {
      sender_addr_real_addr = &((struct sockaddr_in *)sender_addr)->sin_addr;
      sender_addr_real_port = ((struct sockaddr_in *)sender_addr)->sin_port;
    }
  else if (socket_family == AF_INET6)
    {
      sender_addr_real_addr = &((struct sockaddr_in6 *)sender_addr)->sin6_addr;
      sender_addr_real_port = ((struct sockaddr_in6 *)sender_addr)->sin6_port;
    }

  if (inet_ntop (socket_family, sender_addr_real_addr, sender_addr_str,
                 sender_addr_str_len)
      == NULL)
    {
      printf ("cannot convert sender address to string\n");
    }
  else
    {
      printf ("socket received %ld bytes from (%s, %u)\n", n_bytes,
              sender_addr_str, ntohs (sender_addr_real_port));
      printf ("message: ");
      for (ssize_t i = 0; i < n_bytes; ++i)
        {
          printf ("%c", (char)msg[i]);
        }
      printf ("\n");
    }
}

void
make_socket_async (int sfd)
{

  if (fcntl (sfd, F_SETOWN, getpid ()) < 0)
    {
      perror_exit ("fcntl F_SETOWN");
    }

  int flags = fcntl (sfd, F_GETFL);
  if (fcntl (sfd, F_SETFL, flags | FASYNC | FNONBLOCK) < 0)
    {
      perror_exit ("fcntl F_SETFL, FASYNC | FNONBLOCK");
    }
}

int
create_binded_socket (int socket_family, const struct sockaddr_storage *addr,
                      socklen_t socklen)
{
  int sfd = socket (socket_family, SOCK_DGRAM, IPPROTO_UDP);
  if (sfd == -1)
    perror_exit ("cannot create an endpoint for communication");

  if (bind (sfd, (struct sockaddr *)addr, socklen) == -1)
    {
      perror_exit ("cannot bind a name to a socket");
    }

  return sfd;
}
