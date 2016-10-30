#ifndef _NETWORK_CLIENT_PRIVATE_H
#define _NETWORK_CLIENT_PRIVATE_H

#include "inet.h"
#include "network_client.h"
#include <netdb.h>

struct server_t{
        int socket_fd;
  //TODO código nao compila!
	// uint8		sin_len; //Compr. Nao existe no Linux
	// sa_family_t	sin_family;
	// in_port_t	sin_port;
	// struct	in_addr sin_addr;
	// char 		sin_zero[8]; /* nao usado */
};


#endif
