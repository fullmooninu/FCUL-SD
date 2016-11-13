/* Sistemas Distribuidos - 2016 - Grupo 34
Elias Miguel Barreira 40821, Pedro Pais 41375
Silvia Ferreira 45511 */

#ifndef _CLIENT_STUB_PRIV_H
#define _CLIENT_STUB_PRIV_H
#include "message-private.h"
#include "client_stub.h"
#include "network_client-private.h"

/**
 * Struct da rtable_t (interface para a ligaçao
 *
 */
struct rtable_t{
	struct message_t *msg;
	struct server_t *server;
};

#endif
