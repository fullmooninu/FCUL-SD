#ifndef _NETWORK_CLIENT_PRIVATE_H
#define _NETWORK_CLIENT_PRIVATE_H

#include "inet.h"
#include "network_client.h"
#include <netdb.h>

#define RETRY_TIME 6

struct server_t{
        int socket_fd;
};


void print_message(struct message_t *msg);

#endif
