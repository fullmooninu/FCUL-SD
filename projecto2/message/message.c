#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>

#include "message-private.h"

void free_message(struct message_t *msg){

  /* Verificar se msg é NULL */

  /* Se msg->c_type for:
      VALOR, libertar msg->content.data
      ENTRY, libertar msg->content.entry_create
      CHAVES, libertar msg->content.keys
      CHAVE, libertar msg->content.key
  */

  /* libertar msg */

}

int message_to_buffer(struct message_t *msg, char **msg_buf){

  /* Verificar se msg é NULL */
	if(msg == NULL || msg_buf == NULL) return NULL;

  /* Consoante o msg->c_type, determinar o tamanho do vetor de bytes
     que tem de ser alocado antes de serializar msg
  */
	int sizeBytes; //Tamanho em bytes

	switch (msg->c_type)
	{
	case CT_RESULT: sizeBytes = 4;
					break;
	case CT_VALUE: sizeBytes = (4 + sizeof(msg->content->data)); //Verificar se funciona
					break;
	case CT_KEY: sizeBytes = (4 + sizeof(msg->content->key));
					break;
	case CT_KEYS: sizeBytes = (6 + sizeof(msg->content->keys));
					break;
	case CT_ENTRY: sizeBytes = (4 + sizeof(msg->content->key) + 4 + sizeof(msg->content->data));
					break;
	}


  /* Alocar quantidade de memória determinada antes 
     *msg_buf = ....
  */

  /* Inicializar ponteiro auxiliar com o endereço da memória alocada */
  ptr = *msg_buf;

  short_value = htons(msg->opcode);
  memcpy(ptr, &short_v, _SHORT);
  ptr += _SHORT;

  short_value = htons(msg->c_type);
  memcpy(ptr, &short_v, _SHORT);
  ptr += _SHORT;

  /* Consoante o conteúdo da mensagem, continuar a serialização da mesma */

  return buffer_size;
}

struct message_t *buffer_to_message(char *msg_buf, int msg_size){

  /* Verificar se msg_buf é NULL */

  /* msg_size tem tamanho mínimo ? */

  /* Alocar memória para uma struct message_t */

  /* Recuperar o opcode e c_type */
  memcpy(&short_aux, msg_buf, _SHORT);
  msg->opcode = ntohs(short_aux);
  msg_buf += _SHORT;

  memcpy(&short_aux, msg_buf, _SHORT);
  msg->c_type = ntohs(short_aux);
  msg_buf += _SHORT;

  /* A mesma coisa que em cima mas de forma compacta, ao estilo C! */
  msg->opcode = ntohs(*(short *) msg_buf++);
  msg->c_type = ntohs(*(short *) ++msg_buf);
  msg_buf += _SHORT;

  /* O opcode e c_type são válidos? */

  /* Consoante o c_type, continuar a recuperação da mensagem original */

  return msg;
}

