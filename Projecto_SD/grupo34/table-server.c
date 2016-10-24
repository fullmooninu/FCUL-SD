/*
   Programa que implementa um servidor de uma tabela hash com chainning.
   Uso: table-server <porta TCP> <dimensão da tabela>
   Exemplo de uso: ./table_server 54321 10
*/
#include <error.h>

#include "inet.h"
#include "table-private.h"
#include "message-private.h"


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

  if (listen(socket_fd, 0) < 0){
      perror("Erro ao executar listen");
      close(sfd);
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
      result = table_del(tabela, msg_pedido->content.key);
      if (result == -1) {
        msg_resposta->opcode = OC_RT_ERROR;
        msg_resposta->c_type = CT_RESULT;
      } else {
        msg_resposta->opcode = OC_DEL + 1;
        msg_resposta->c_type = CT_RESULT
      }
      msg_resposta->conten.result = result;
    break;
    case OC_UPDATE:
      //TODO
    break;
    case OC_GET:
      //TODO
    break;
    case OC_PUT:
      //TODO
    break;
  }



	return msg_resposta;
}


/* Função "inversa" da função network_send_receive usada no table-client.
   Neste caso a função implementa um ciclo receive/send:

	Recebe um pedido;
	Aplica o pedido na tabela;
	Envia a resposta.
*/
int network_receive_send(int sockfd, struct table_t *table){
  char *message_resposta, *message_pedido;
  int msg_length;
  int message_size, msg_size, result;
  struct message_t *msg_pedido, *msg_resposta;
  struct list_t *results;

	/* Verificar parâmetros de entrada */

	/* Com a função read_all, receber num inteiro o tamanho da
	   mensagem de pedido que será recebida de seguida.*/
	result = read_all(sockfd, (char *) &msg_size, _INT);

	/* Verificar se a receção teve sucesso */
  if (result == -1) return -1;

	/* Alocar memória para receber o número de bytes da
	   mensagem de pedido. */
  message_pedido = malloc(sizeof(char)*msg_size);

	/* Com a função read_all, receber a mensagem de resposta. */
	result = read_all(sockfd, message_pedido, msg_size);

	/* Verificar se a receção teve sucesso */
  //TODO libertar memoria em caso de erro
  if (result == -1) return -1;

	/* Desserializar a mensagem do pedido */
	msg_pedido = buffer_to_message(message_pedido, msg_size);

	/* Verificar se a desserialização teve sucesso */
  //TODO libertar memoria em caso de erro
  if (msg_pedido == NULL) return NULL;

	/* Processar a mensagem */
	msg_resposta = process_message(msg_pedido, table);

	/* Serializar a mensagem recebida */
	message_size = message_to_buffer(msg_resposta, &message_resposta);

	/* Verificar se a serialização teve sucesso */
  //TODO libertar memoria em caso de erro
  if (message_size == -1) return -1;

	/* Enviar ao cliente o tamanho da mensagem que será enviada
	   logo de seguida
	*/
	msg_size = htonl(message_size);
  //TODO DUVIDA ponteiro 'server' vem de onde?
 	result = write_all(server->/*atributo*/, (char *) &msg_size, _INT));

	/* Verificar se o envio teve sucesso */
  //TODO libertar memoria em caso de erro
  if (result == -1) return -1;

	/* Enviar a mensagem que foi previamente serializada */

	result = write_all(server->/*atributo*/, message_resposta, message_size));

	/* Verificar se o envio teve sucesso */
  //TODO libertar memoria em caso de erro
  if (result == -1) return -1;

	/* Libertar memória */
  free(message_pedido);
  free(message_resposta);
  free_message(msg_pedido); //message.h
  free_message(msg_resposta); //message.h

	return 0;
}



int main(int argc, char **argv){
	int listening_socket, connsock, result;
	struct sockaddr_in client;
	socklen_t size_client;
	struct table_t *table;

	if (argc != 2){
	printf("Uso: ./server <porta TCP> <dimensão da tabela>\n");
	printf("Exemplo de uso: ./table-server 54321 10\n");
	return -1;
	}

	if ((listening_socket = make_server_socket(atoi(argv[1]))) < 0) return -1;

	if ((table = table_create(atoi(argv[2]))) == NULL){
	result = close(listening_socket);
	return -1;
	}

	while ((connsock = accept(listening_socket, (struct sockaddr *) &client, &size_client)) != -1) {
		printf(" * Client is connected!\n");

		while (/* condição */){

			/* Fazer ciclo de pedido e resposta */
			network_receive_send(connsock, table) < 0);

			/* Ciclo feito com sucesso ? Houve erro?
			   Cliente desligou? */

		}
	}
}