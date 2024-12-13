#pragma once

#include <netinet/in.h>
#include <sys/socket.h>

void init_sockaddr_storage_in (struct sockaddr_storage *addr,
                               socklen_t *socklen, in_port_t port);
void init_sockaddr_storage_in6 (struct sockaddr_storage *addr,
                                socklen_t *socklen, in_port_t port);

void str2port (const char *s, in_port_t *port);

void print_recv_info (uint8_t *msg, ssize_t n_bytes, int socket_family,
                      const struct sockaddr_storage *sender_addr,
                      char *sender_addr_str, const size_t sender_addr_str_len);

void make_socket_async (int sfd);

int create_binded_socket (int socket_family,
                          const struct sockaddr_storage *addr,
                          socklen_t socklen);
