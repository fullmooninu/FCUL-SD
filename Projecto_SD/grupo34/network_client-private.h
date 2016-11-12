#ifndef _NETWORK_CLIENT_PRIVATE_H
#define _NETWORK_CLIENT_PRIVATE_H

#include "inet.h"
#include "network_client.h"
#include <netdb.h>

struct server_t{
    int socket_fd;
	int	port;
	int addrIP;
	struct sockaddr_in addr;
};


#endif
