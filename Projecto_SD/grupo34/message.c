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


  /* Consoante o msg->c_type, determinar o tamanho do vetor de bytes
  que tem de ser alocado antes de serializar msg
  */

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
  if (msg_buf == NULL) return NULL;

  /* msg_size tem tamanho mínimo ? */
  if(msg_size < 7) return NULL;

  /* Alocar memória para uma struct message_t */
  struct message_t* msg;
  msg = (struct message_t *) malloc(sizeof(struct message_t));
  /* Recuperar o opcode e c_type */
  short short_aux;
  //short_aux = ntohs(msg->opcode);
  memcpy(&short_aux, msg_buf, _SHORT);
  msg->opcode = ntohs(short_aux);
  msg_buf += _SHORT;

  memcpy(&short_aux, msg_buf, _SHORT);
  msg->c_type = ntohs(short_aux);
  msg_buf += _SHORT;

  /* A mesma coisa que em cima mas de forma compacta, ao estilo C! */
  /*msg->opcode = ntohs(*(short *) msg_buf++);
  msg->c_type = ntohs(*(short *) ++msg_buf);
  msg_buf += _SHORT;
  */
  /* O opcode e c_type são válidos? */
  if(!isValidOC(msg->opcode) || !isValidCTC (msg->c_type)) return NULL;

  /* Consoante o c_type, continuar a recuperação da mensagem original */
  switch (msg->c_type) {
    int int_aux;
    case CT_RESULT:
    break;
    case CT_VALUE:
      struct data_t* value;
      memcpy(&int_aux, msg_buf,_INT);
      msg->c_type = ntohs(int_aux);
      msg_buf += _INT;
      //como traduz da network p host???
      value = data_create2(int_aux, msg_buf);
      msg->data = value;
      break;
    case CT_KEY:
      char *key;
      memcpy(&short_aux, msg_buf,_SHORT);
      msg->c_type = ntohs(short_aux);
      msg_buf += _SHORT;
      //como traduz p host???
      key = (char*) malloc(sizeof(char)*short_aux);
      strcpy(key, msg_buf, short_aux);
      msg->key = key;
      break;
    case CT_KEYS:
      char** keys;
      memcpy(&int_aux, msg_buf,_INT);
      msg->c_type = ntohs(int_aux);
      //msg_buf += _INT;
//como traduz
      keys = (char**) malloc(sizeof(char)*int_aux);
      int i;
      for(i = 0; i <int_aux; i++){
        msg_buf += _SHORT;
        memcpy(&short_aux, msg_buf,_INT);
        short key_size;
        key_size = short_aux;
        msg_buf += _SHORT;
        keys[i] = strndup(msg_buf, ntohs(short_aux));
        msg_buf +=_SHORT;
      }
      msg->keys = keys;
      break;
    default:
      struct entry_t* entry;
      struct data* value;
      memcpy(&int_aux, msg_buf,_INT);
      msg->c_type = ntohs(int_aux);
      msg_buf += _INT;
      //como traduz da network p host???
      value = data_create2(int_aux, msg_buf);
      msg->data = value;
  }
  return msg;
}
