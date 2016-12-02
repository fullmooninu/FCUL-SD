#ifndef _NETWORK_CLIENT_PRIVATE_H
#define _NETWORK_CLIENT_PRIVATE_H

#include "inet.h"
#include "network_client.h"
#include <netdb.h>

#define RETRY_TIME 2

struct server_t{
    int socket_fd;
    struct sockaddr_in server_in;
    char *IP;
	int port;
};


#endif
