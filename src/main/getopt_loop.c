#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>

#include "globals.h"
#include "log.h"
#include "socket_helpers.h"

#include "getopt_loop.h"

void
getopt_loop (int argc, char *argv[], const char *short_options,
             const struct option *long_options,
             const size_t required_options_size,
             const char *required_options[])
{
  size_t required_options_count = 0;
  bool required_option_presence[required_options_size];
  int opt, long_opt_idx;
  while ((opt = getopt_long (argc, argv, short_options, long_options,
                             &long_opt_idx))
         != -1)

    {

      count_required_option (opt, required_options_size, required_options,
                             required_option_presence,
                             &required_options_count);

      switch (opt)
        {
        case 'c':
          {
            is_client = true;
            open_flag = O_RDONLY;
            state = STATE_SENDING_PACKET_COUNT;
            break;
          }
        case 's':
          {
            is_client = false;
            open_flag = O_WRONLY | O_CREAT | O_TRUNC;
            state = STATE_UNSPEC;
            break;
          }
          {
            void *addr_real_addr = NULL;
          case '4':
            if (opt == '4')
              {
                if (is_client)
                  {
                    remote_addr_in.sin_family = AF_INET;
                    remote_addr_in.sin_port = htons (remote_port);
                    remote_socklen = sizeof (struct sockaddr_in);
                    remote_addr = (struct sockaddr_storage *)&addr_in;

                    addr_real_addr = &remote_addr_in.sin_addr;
                  }

                init_sockaddr_storage_in (&addr, &socklen, &addr_in, port);
              }
          case '6':
            if (opt == '6')
              {
                if (is_client)
                  {
                    remote_addr_in6.sin6_family = AF_INET6;
                    remote_addr_in6.sin6_port = htons (remote_port);
                    remote_socklen = sizeof (struct sockaddr_in6);
                    remote_addr = (struct sockaddr_storage *)&addr_in6;

                    addr_real_addr = &remote_addr_in6.sin6_addr;
                  }

                init_sockaddr_storage_in6 (&addr, &socklen, &addr_in6, port);
              }

            if (is_client)
              {
                if (inet_pton (socket_family, optarg, addr_real_addr) != 1)
                  {
                    error_exit ("cannot convert a '%s' address passed with "
                                "option '%s' from"
                                "text to binary form\n",
                                optarg, long_options[long_opt_idx].name);
                  }
              }
          }
        case 'p':
          {
            str2port (optarg, &port);
            break;
          }
        case 'r':
          {
            str2port (optarg, &remote_port);
            break;
          }
        case 'f':
          {
            filename = (const char *)optarg;
            break;
          }
        case ':':
          {
            error_exit ("option needs a value\n");
            break;
          }
        case '?':
          {
            error_exit ("unknown option : %c\n", optopt);
            break;
          }
        }
    }

  if (required_options_count < required_options_size)
    {
      for (size_t i = 0; i < required_options_size; ++i)
        {
          if (!required_option_presence[i])
            {
              // each character in required_options[i] is a short option
              error_exit ("missing required option: %s", required_options[i]);
            }
        }
    }
}

// checking if an option is required
// if an option is required then
// mark it's presence in the required_option_presence array
// and increment required_options_count
void
count_required_option (int opt, const size_t required_options_size,
                       const char *required_options[required_options_size],
                       bool required_option_presence[required_options_size],
                       size_t *required_options_count)
{
  for (size_t i = 0; (*required_options_count) < required_options_size
                     && i < required_options_size;
       ++i)
    {
      if (!required_option_presence[i])
        {
          for (const char *end = required_options[i]; (*end) != '\0'; ++end)
            {
              if (opt == (*end))
                {
                  ++(*required_options_count);
                  required_option_presence[i] = true;
                  break;
                }
            }
        }
    }
}
