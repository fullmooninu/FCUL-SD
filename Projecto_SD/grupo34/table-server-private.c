
#include <stdlib.h>
#include "table-server-private.h"

/* Libertar memória alocada no network_receive_send do table-server*/
void free_memory( char *message_resposta, char *message_pedido, struct message_t* msg_pedido, struct message_t* msg_resposta){
  free(message_pedido);
  free(message_resposta);
  free_message(msg_pedido);
  free_message(msg_resposta);
}
