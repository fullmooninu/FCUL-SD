#include <stdlib.h>
#include <string.h>
#include "table_skel.h"
#include "message-private.h"
#include "table-private.h"
#include <stdio.h>

struct table_t *tabela;

int table_skel_init(int n_lists) {
  if ((tabela = table_create(n_lists)) == NULL) {
    return -1;
  }
  return 0;
}

/* Libertar toda a memória e recursos alocados pela função anterior.
 */
int table_skel_destroy() {
  table_destroy(tabela);
  return 0;
}

void print_table() {
  char** table_keys;
  struct data_t *data;
  int i, j;
  char *c;

  table_keys = table_get_keys(tabela);
    if (table_keys != NULL) {
      for(i = 0; table_keys[i] != NULL; i++) {
        fflush(stdout);
        data = table_get(tabela, table_keys[i]);
        printf("key: %s - value: ", table_keys[i]);
        c = (char *) data->data;
        for (j = 0; j < data->datasize; j++) {
      		printf("%c", *c);
      		c += 1;
      	}
      	printf("\n");
        fflush(stdout);
        data_destroy(data);
      }
      table_free_keys(table_keys);
    }
}


/* Função que recebe uma tabela e uma mensagem de pedido e:
- aplica a operação na mensagem de pedido na tabela;
- devolve uma mensagem de resposta com oresultado.
*/
struct message_t *invoke(struct message_t *msg_pedido){
  struct message_t *msg_resposta;
  int result;
  struct data_t* data;
  /* Verificar parâmetros de entrada */
  if (msg_pedido == NULL || tabela == NULL) return NULL;

  msg_resposta = (struct message_t *) malloc(sizeof(struct message_t));

  /* Verificar opcode e c_type na mensagem de pedido */
  /* Aplicar operação na tabela */
  /* Preparar mensagem de resposta */
  switch (msg_pedido->opcode) {
    case OC_SIZE:
    msg_resposta->opcode = OC_SIZE + 1;
    msg_resposta->c_type = CT_RESULT;
    msg_resposta->content.result = table_size(tabela);
    break;
    case OC_DEL:
    //Garantir o c_type correspondente ao comando
    if(msg_pedido->c_type == CT_KEY){
      result = table_del(tabela, msg_pedido->content.key);
      if (result == -1) {
        msg_resposta->opcode = OC_RT_ERROR;
        msg_resposta->c_type = CT_RESULT;
      } else {
        msg_resposta->opcode = OC_DEL + 1;
        msg_resposta->c_type = CT_RESULT;
      }
      msg_resposta->content.result = result;
    }else{
      msg_resposta->opcode = OC_RT_ERROR;
      msg_resposta->c_type = CT_RESULT;
      msg_resposta->content.result = -1;
    }
    break;
    case OC_UPDATE:
    //Garantir o c_type correspondente ao comando
    if(msg_pedido->c_type == CT_ENTRY){
      result = table_update(tabela, msg_pedido->content.entry->key, msg_pedido->content.entry->value);
      if (result == -1) {
        msg_resposta->opcode = OC_RT_ERROR;
        msg_resposta->c_type = CT_RESULT;
      } else {
        msg_resposta->opcode = OC_UPDATE+1;
        msg_resposta->c_type = CT_RESULT;
      }
      msg_resposta->content.result = result;
    }else{
      msg_resposta->opcode = OC_RT_ERROR;
      msg_resposta->c_type = CT_RESULT;
      msg_resposta->content.result = -1;
    }
    break;
    case OC_GET:
    //Garantir o c_type correspondente ao comando
    if(msg_pedido->c_type == CT_KEY){
      char* key;
      msg_resposta->opcode = OC_GET+1;
      key = msg_pedido->content.key;
      if(strcmp(key, "*") == 0){
        msg_resposta->c_type = CT_KEYS;
        msg_resposta->content.keys = table_get_keys(tabela);
      }else {
        msg_resposta->c_type = CT_VALUE;
        data = table_get(tabela, key);
        if(data == NULL){
          // data = data_create(0);
          data =  (struct data_t*) malloc(sizeof(struct data_t));
          if (data == NULL) return NULL;
          data->datasize = 0;
          data->data = NULL;
        }
        msg_resposta->content.data = data;
      }
    }else{
      msg_resposta->opcode = OC_RT_ERROR;
      msg_resposta->c_type = CT_RESULT;
      msg_resposta->content.result = -1;
    }
    break;
    case OC_PUT:
    if(msg_pedido->c_type == CT_ENTRY){
      result = table_put(tabela, msg_pedido->content.entry->key, msg_pedido->content.entry->value);
      if (result == -1) {
        msg_resposta->opcode = OC_RT_ERROR;
        msg_resposta->c_type = CT_RESULT;
      } else {
        msg_resposta->opcode = OC_PUT+1;
        msg_resposta->c_type = CT_RESULT;
      }
      msg_resposta->content.result = result;
    }else{
      msg_resposta->opcode = OC_RT_ERROR;
      msg_resposta->c_type = CT_RESULT;
      msg_resposta->content.result = -1;
    }
    break;
  }
  return msg_resposta;
}
