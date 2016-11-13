#include <stdlib.h>
#include "table_server-private.h"

/* Libertar memória alocada no network_receive_send do table-server*/
void free_memory( char *message_resposta, char *message_pedido, struct message_t* msg_pedido, struct message_t* msg_resposta){

  if(message_pedido != NULL){
    free(message_pedido);
  }
  if(message_resposta != NULL){
    free(message_resposta);
  }
  if(msg_pedido != NULL){
    free_message(msg_pedido);
  }
  if(msg_resposta != NULL){
    free_message(msg_resposta);
  }

}
