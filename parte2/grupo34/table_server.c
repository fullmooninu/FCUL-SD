/* Sistemas Distribuidos - 2016 - Grupo 34
Elias Miguel Barreira 40821, Pedro Pais 41375
Silvia Ferreira 45511 */

/*
Programa que implementa um servidor de uma tabela hash com chainning.
Uso: table-server <porta TCP> <dimensão da tabela>
Exemplo de uso: ./table_server 54321 10
*/
#include <error.h>
#include "inet.h"
#include "table-private.h"
#include "message-private.h"
#include "table_server-private.h"
//#include "message.h"
#define MAX_CLIENTES 1

/* Função para preparar uma socket de receção de pedidos de ligação.
*/
int make_server_socket(short port){
  int socket_fd;
  struct sockaddr_in server;

  if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
    perror("Erro ao criar socket");
    return -1;
  }

  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  server.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(socket_fd, (struct sockaddr *) &server, sizeof(server)) < 0){
    perror("Erro ao fazer bind");
    close(socket_fd);
    return -1;
  }

  if (listen(socket_fd, MAX_CLIENTES) < 0){
    perror("Erro ao executar listen");
    close(socket_fd);
    return -1;
  }
  return socket_fd;
}


/* Função que recebe uma tabela e uma mensagem de pedido e:
- aplica a operação na mensagem de pedido na tabela;
- devolve uma mensagem de resposta com oresultado.
*/
struct message_t *process_message(struct message_t *msg_pedido, struct table_t *tabela){
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
          data = data_create(0);
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


/* Função "inversa" da função network_send_receive usada no table-client.
Neste caso a função implementa um ciclo receive/send:

Recebe um pedido;
Aplica o pedido na tabela;
Envia a resposta.#include "message.h"

*/
int network_receive_send(int sockfd, struct table_t *table){
  char *message_resposta = NULL;
  char *message_pedido = NULL;
  //int msg_length;
  int host_size, net_size, result;
  struct message_t *msg_pedido = NULL;
  struct message_t *msg_resposta = NULL;
  //int msg_length;
  //struct list_t *results;

  /* Verificar parâmetros de entrada */

  if(sockfd < 0|| table == NULL) return -1;


  /* Com a função read_all, receber num inteiro o tamanho da
  mensagem de pedido que será recebida de seguida.*/
  printf("antes do read_all\n");
  fflush(stdout);

  result = read_all(sockfd, (char *) &net_size, _INT);
  /* Verificar se a receção teve sucesso */
  if (result == -1) return -1;

  host_size = ntohl(net_size);
  printf("host size: %d\n", host_size);
  printf("net size: %d\n", net_size);
  printf("read_all bytes lidos: %d\n", result);
  fflush(stdout);
  /* Alocar memória para receber o número de bytes da
  mensagem de pedido. */
  message_pedido = malloc(sizeof(char)*host_size);

  /* Com a função read_all, receber a mensagem de resposta. */
  result = read_all(sockfd, message_pedido, host_size);
  printf("bytes recebidos buffer: %d\n", result);
  fflush(stdout);

  /* Verificar se a receção teve sucesso */
  if (result == -1){
    free_memory(message_resposta, message_pedido, msg_pedido, msg_resposta);
    return -1;
  }

  /* Desserializar a mensagem do pedido */
  msg_pedido = buffer_to_message(message_pedido, host_size);

  /* Verificar se a desserialização teve sucesso */
  if (msg_pedido == NULL){
    free_memory(message_resposta, message_pedido, msg_pedido, msg_resposta);
    return -1;
  }

  /* Processar a mensagem */
  msg_resposta = process_message(msg_pedido, table);

  /* Serializar a mensagem recebida */
  host_size = message_to_buffer(msg_resposta, &message_resposta);

  /* Verificar se a serialização teve sucesso */
  if (host_size == -1){
    free_memory(message_resposta, message_pedido, msg_pedido, msg_resposta);
    return -1;
  }

  /* Enviar ao cliente o tamanho da mensagem que será enviada
  logo de seguida
  */
  net_size = htonl(host_size);
  result = write_all(sockfd, (char *) &net_size, _INT);

  /* Verificar se o envio teve sucesso */
  if (result == -1){
    free_memory(message_resposta, message_pedido, msg_pedido, msg_resposta);
    return -1;
  }

  /* Enviar a mensagem que foi previamente serializada */

  result = write_all(sockfd, message_resposta, host_size);

  /* Verificar se o envio teve sucesso */
  if (result == -1) {
    free_memory(message_resposta, message_pedido, msg_pedido, msg_resposta);
    return -1;
  }

  /* Libertar memória */
  free_memory(message_resposta, message_pedido, msg_pedido, msg_resposta);


  return 0;
}


int main(int argc, char **argv){
  int listening_socket, connsock;
  struct sockaddr_in client;
  socklen_t size_client;
  struct table_t *table;
  //struct message_t* msg_resposta;

  if (argc != 3){
    printf("Uso: ./server <porta TCP> <dimensão da tabela>\n");
    printf("Exemplo de uso: ./table-server 54321 10\n");
    return -1;
  }

  if ((listening_socket = make_server_socket(atoi(argv[1]))) < 0) {
    return -1;
  }

  if ((table = table_create(atoi(argv[2]))) == NULL){
    int result;
    result = close(listening_socket);
    return result;
  }

  while ((connsock = accept(listening_socket, (struct sockaddr *) &client, &size_client)) != -1) {
    printf(" * Client is connected!\n");

    //true
    int success = -1;
    do {
      //msg_resposta->c_type = CT_VALUE;
      success = network_receive_send(connsock, table);
    }while (success!=-1);
    /* Fazer ciclo de pedido e resposta */


    //TODO
    /* Ciclo feito com sucesso ? Houve erro?
    Cliente desligou? */

  }
}
