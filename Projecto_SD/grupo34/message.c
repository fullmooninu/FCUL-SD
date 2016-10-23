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
	int buffer_size; //Tamanho em bytes

	//REVER TAMANHOS
	switch (msg->c_type)
	{
	case CT_ENTRY: buffer_size = (6 + sizeof(msg->content->key) + 4 + sizeof(msg->content->struct entry_t));
							break;
	case CT_KEY: buffer_size = (6 + sizeof(msg->content->key));
							break;
	case CT_KEYS: buffer_size = (10 + sizeof(msg->content->keys));
							break;
	case CT_VALUE: buffer_size = (8 + sizeof(msg->content->struct data_t)); //Verificar se funciona
						break;
	case CT_RESULT: buffer_size = 8;
						break;
	}
  /* Alocar quantidade de memória determinada antes */
  msg_buf = malloc(sizeBytes);


  /* Inicializar ponteiro auxiliar com o endereço da memória alocada */
  ptr = *msg_buf;

  short short_value = htons(msg->opcode);
  memcpy(ptr, &short_value, _SHORT);
  ptr += _SHORT;

  short short_value = htons(msg->c_type);
  memcpy(ptr, &short_value, _SHORT);
  ptr += _SHORT;

  /* Consoante o conteúdo da mensagem, continuar a serialização da mesma */

  switch(msg->c_type)
  int int_aux;
  {
  case CT_ENTRY:
	  short key_size = strlen(msg->content->key);
	  memcpy(ptr, &key_size, _SHORT);
	  ptr += _SHORT;
	  //colocar a chave
	  char *key_aux;
	  key_aux=(char *)malloc(sizeof(msg->content->key));
	  strcpy(key_aux, msg->content->key);
	  memcpy(ptr, &key_aux, strlen(key_aux));
	  ptr += strlen(key_aux); //j� sabemos que o strlen nao contabiliza o '/0'
	  free(key_aux);
	  //colocar o data_sizeDS
	  int_aux = sizeof(msg->content->struct data_t);
	  int int_dataSize = htons(int_aux);
	  memcpy(ptr, &int_dataSize, _INT);
	  ptr += _INT;
	  //colocar o data

	  break;
  case CT_KEY:
	  break;
  case CT_KEYS:
	  break;
  case CT_VALUE:
	  break;
  default: //Default � o CT_RESULT **
	  int int_result = htons(msg->content->result);
	  memcpy(ptr, &int_result, _INT);
	  ptr += _INT;
	  break;
  }

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
