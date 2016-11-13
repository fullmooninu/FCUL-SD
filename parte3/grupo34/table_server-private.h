#ifndef _TABLE_SERVER_PRIV_H
#define _TABLE_SERVER_PRIV_H

#include "message.h"

void free_memory( char *message_resposta, char *message_pedido, struct message_t* msg_pedido, struct message_t* msg_resposta);

#endif
