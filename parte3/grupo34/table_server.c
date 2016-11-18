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
#include "table_skel.h"
#include <poll.h>

#define MAX_CLIENTES 5

/* Função para preparar uma socket de receção de pedidos de ligação.
 */
int make_server_socket(short port) {
	int socket_fd;
	struct sockaddr_in server;

	if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Erro ao criar socket");
		return -1;
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(socket_fd, (struct sockaddr *) &server, sizeof(server)) < 0) {
		perror("Erro ao fazer bind");
		close(socket_fd);
		return -1;
	}

	if (listen(socket_fd, MAX_CLIENTES) < 0) {
		perror("Erro ao executar listen");
		close(socket_fd);
		return -1;
	}
	return socket_fd;
}

//
///* Função que recebe uma tabela e uma mensagem de pedido e:
//- aplica a operação na mensagem de pedido na tabela;
//- devolve uma mensagem de resposta com oresultado.
//*/
//struct message_t *process_message(struct message_t *msg_pedido, struct table_t *tabela){
//  struct message_t *msg_resposta;
//  int result;
//  struct data_t* data;
//  char* key;
//  /* Verificar parâmetros de entrada */
//  if (msg_pedido == NULL || tabela == NULL) return NULL;
//
//  msg_resposta = (struct message_t *) malloc(sizeof(struct message_t));
//
//  /* Verificar opcode e c_type na mensagem de pedido */
//  /* Aplicar operação na tabela */
//  /* Preparar mensagem de resposta */
//  switch (msg_pedido->opcode) {
//    case OC_SIZE:
//    msg_resposta->opcode = OC_SIZE + 1;
//    msg_resposta->c_type = CT_RESULT;
//    msg_resposta->content.result = table_size(tabela);
//    break;
//    case OC_DEL:
//    //Garantir o c_type correspondente ao comando
//    if(msg_pedido->c_type == CT_KEY){
//      result = table_del(tabela, msg_pedido->content.key);
//      if (result == -1) {
//        msg_resposta->opcode = OC_RT_ERROR;
//        msg_resposta->c_type = CT_RESULT;
//      } else {
//        msg_resposta->opcode = OC_DEL + 1;
//        msg_resposta->c_type = CT_RESULT;
//      }
//      msg_resposta->content.result = result;
//    }else{
//      msg_resposta->opcode = OC_RT_ERROR;
//      msg_resposta->c_type = CT_RESULT;
//      msg_resposta->content.result = -1;
//    }
//    break;
//    case OC_UPDATE:
//    //Garantir o c_type correspondente ao comando
//    if(msg_pedido->c_type == CT_ENTRY){
//      result = table_update(tabela, msg_pedido->content.key, msg_pedido->content.data);
//      if (result == -1) {
//        msg_resposta->opcode = OC_RT_ERROR;
//        msg_resposta->c_type = CT_RESULT;
//      } else {
//        msg_resposta->opcode = OC_UPDATE+1;
//        msg_resposta->c_type = CT_RESULT;
//      }
//      msg_resposta->content.result = result;
//    }else{
//      msg_resposta->opcode = OC_RT_ERROR;
//      msg_resposta->c_type = CT_RESULT;
//      msg_resposta->content.result = -1;
//    }
//    break;
//    case OC_GET:
//    //Garantir o c_type correspondente ao comando
//    if(msg_pedido->c_type == CT_KEY){
//      msg_resposta->opcode = OC_GET+1;
//      key = msg_pedido->content.key;
//      if(strcmp(key, "*")){
//        msg_resposta->c_type = CT_KEYS;
//        msg_resposta->content.keys = table_get_keys(tabela);
//      }else {
//        msg_resposta->c_type = CT_VALUE;
//        data = table_get(tabela, key);
//        if(data == NULL){
//          data = (struct data_t*) malloc(sizeof(struct data_t));
//          data->datasize = 0;
//          data->data = NULL;
//        }
//        msg_resposta->content.data = data;
//      }
//    }else{
//      msg_resposta->opcode = OC_RT_ERROR;
//      msg_resposta->c_type = CT_RESULT;
//      msg_resposta->content.result = -1;
//    }
//    break;
//    case OC_PUT:
//    if(msg_pedido->c_type == CT_ENTRY){
//      result = table_put(tabela, msg_pedido->content.key, msg_pedido->content.data);
//      if (result == -1) {
//        msg_resposta->opcode = OC_RT_ERROR;
//        msg_resposta->c_type = CT_RESULT;
//      } else {
//        msg_resposta->opcode = OC_PUT+1;
//        msg_resposta->c_type = CT_RESULT;
//      }
//      msg_resposta->content.result = result;
//    }else{
//      msg_resposta->opcode = OC_RT_ERROR;
//      msg_resposta->c_type = CT_RESULT;
//      msg_resposta->content.result = -1;
//    }
//    break;
//  }
//  return msg_resposta;
//}

///* Função "inversa" da função network_send_receive usada no table-client.
//Neste caso a função implementa um ciclo receive/send:
//
//Recebe um pedido;
//Aplica o pedido na tabela;
//Envia a resposta.#include "message.h"
//
//*/
//int network_receive_send(int sockfd, struct table_t *table){
//  char *message_resposta = NULL;
//  char *message_pedido = NULL;
//  //int msg_length;
//  int host_size, net_size, result;
//  struct message_t *msg_pedido = NULL;
//  struct msg_t *msg_resposta = NULL;
//  //int msg_length;
//  struct list_t *results;
//
//  /* Verificar parâmetros de entrada */
//
//  if(sockfd < 0|| table == NULL) return -1;
//
//
//  /* Com a função read_all, receber num inteiro o tamanho da
//  mensagem de pedido que será recebida de seguida.*/
//  printf("antes do read_all\n");
//	fflush(stdout);
//
//  result = read_all(sockfd, (char *) &net_size, _INT);
//  /* Verificar se a receção teve sucesso */
//  if (result == -1) return -1;
//
//  host_size = ntohl(net_size);
//  printf("host size: %d\n", host_size);
//  printf("net size: %d\n", net_size);
//  printf("read_all bytes lidos: %d\n", result);
//  fflush(stdout);
//  /* Alocar memória para receber o número de bytes da
//  mensagem de pedido. */
//  message_pedido = malloc(sizeof(char)*host_size);
//
//  /* Com a função read_all, receber a mensagem de resposta. */
//  result = read_all(sockfd, message_pedido, host_size);
//  printf("bytes recebidos buffer: %d\n", result);
//	fflush(stdout);
//
//  /* Verificar se a receção teve sucesso */
//  if (result == -1){
//    free_memory(message_resposta, message_pedido, msg_pedido, msg_resposta);
//    return -1;
//  }
//
//  /* Desserializar a mensagem do pedido */
//  msg_pedido = buffer_to_message(message_pedido, host_size);
//
//  /* Verificar se a desserialização teve sucesso */
//  if (msg_pedido == NULL){
//    free_memory(message_resposta, message_pedido, msg_pedido, msg_resposta);
//    return -1;
//  }
//
//  /* Processar a mensagem */
//  msg_resposta = process_message(msg_pedido, table);
//
//  /* Serializar a mensagem recebida */
//  host_size = message_to_buffer(msg_resposta, &message_resposta);
//
//  /* Verificar se a serialização teve sucesso */
//  if (host_size == -1){
//    free_memory(message_resposta, message_pedido, msg_pedido, msg_resposta);
//    return -1;
//  }
//
//  /* Enviar ao cliente o tamanho da mensagem que será enviada
//  logo de seguida
//  */
//  net_size = htonl(host_size);
//  result = write_all(sockfd, (char *) &net_size, _INT);
//
//  /* Verificar se o envio teve sucesso */
//  if (result == -1){
//    free_memory(message_resposta, message_pedido, msg_pedido, msg_resposta);
//    return -1;
//  }
//
//  /* Enviar a mensagem que foi previamente serializada */
//
//  result = write_all(sockfd, message_resposta, host_size);
//
//  /* Verificar se o envio teve sucesso */
//  if (result == -1) {
//    return -1;
//    free_memory(message_resposta, message_pedido, msg_pedido, msg_resposta);
//  }
//
//  /* Libertar memória */
//  free_memory(message_resposta, message_pedido, msg_pedido, msg_resposta);
//
//
//  return 0;
//}

struct rtable_t *server;

int main(int argc, char **argv) {

	int listening_socket, connsock;
	int nBytes, msg_size, count = 2, result;
	struct sockaddr_in client;
	socklen_t size_client = sizeof(struct sockaddr_in);
	struct table_t *table = (struct table_t*) malloc(sizeof(struct table_t));
	char *terminal = malloc(sizeof(char));
	struct message_t* msg_resposta;
	struct message_t* msg_resultado;

	struct pollfd desc_set[MAX_CLIENTES]; //Definido para 5 utilizadores no máximo


	if (argc != 3) {
		printf("Uso: ./server <porta TCP> <dimensão da tabela>\n");
		printf("Exemplo de uso: ./table-server 54321 10\n");
		return -1;
	}

	if ((listening_socket = make_server_socket(atoi(argv[1]))) < 0)
		return -1;

	//Listen
//	if (listen(listening_socket, 20) < 0) {
//		perror("Erro ao executar listen");
//		close(listening_socket);
//		return -1;
//	}

	if ((result = table_skel_init(atoi(argv[2]))) == -1) {
		close(listening_socket);
		return -1;
	}

	desc_set[0].fd = fileno(stdin);
	desc_set[0].events = POLLIN;
	desc_set[0].revents = 0;
	desc_set[1].fd = listening_socket;
	desc_set[1].events = POLLIN;
	desc_set[1].revents = 0;

	//Atribuir os valores aos restantes descritores
	for (int i = 2; i < MAX_CLIENTES; i++) {
		desc_set[i].fd = -1;
		desc_set[i].events = POLLIN;
		desc_set[i].revents = 0;
	}

	//Espera dados nos sockets abertos
	while (poll(desc_set, MAX_CLIENTES, -1) >= 0) {
		//Nova ligaçao
		if (desc_set[1].revents & POLLIN) {
			printf("ENTROU AQUI\n");

			connsock = accept(desc_set[1].fd, (struct sockaddr *) &client,
					&size_client);
			printf("%d",connsock);
			//Adiciona a lista de descritores
			desc_set[2].fd = connsock;
			desc_set[2].events = POLLIN;
		}

		//Testar se há algum evento extra
		if (desc_set[0].revents & POLLIN) {
			fgets(terminal, MAX_MSG, stdin);
			terminal = strtok(terminal, "\n");
			if (strcmp(terminal, "print") == 0) {
				int conta = 0;
				char **tabela;
				tabela = table_get_keys(table);
				printf("A tabela actual é: \n");
				while (tabela[conta] != NULL) {
					printf("Key -> %s", tabela[conta]);
					conta++;
				}
			} else {
				perror(
						"Escreva print para apresentar o conteudo actual do servidor!\n");
			}
		}

		//Ainda do descritor do connection socket e afins
		for (int i = 2; i < count; i++) {
		if (desc_set[i].revents & POLLIN) {
			//Lê string enviada pelo cliente do socket referente a conexão
			if((nBytes = read(desc_set[i].fd, &msg_size, sizeof(int))) <= 0) {
				close(desc_set[i].fd);
				continue;
			}

			msg_size = ntohl(msg_size);

			char *msg_buf=(char *)malloc(msg_size+1);

			msg_resposta = buffer_to_message(msg_buf, msg_size);
			free(msg_buf);

			msg_resultado = invoke(msg_resposta);

			//Envia resposta ao cliente depois de converter para formato de rede
			msg_size = message_to_buffer(msg_resultado, &msg_buf);
			msg_size = htonl(msg_size);

			free(msg_resultado);

			//Envia tamanho do buffer ao cliente através do socket de conexão
			if ((nBytes = write(desc_set[i].fd, (char *) &msg_size, sizeof(int)))
			!= sizeof(int)) {
				perror("Erro ao enviar resposta ao cliente!\n");
				close(desc_set[i].fd);
				continue;
			}

			msg_size = ntohl(msg_size);
			if ((nBytes = write_all(desc_set[i].fd, msg_buf, msg_size)
					!= msg_size)) {
				perror("Erro ao enviar resposta ao cliente!\n");
				close(desc_set[i].fd);
				continue;
			}

			//Verificar se a ligação foi interrompida
			if (desc_set[i].revents && POLLHUP) {
				close(desc_set[i].fd);
				desc_set[i].fd = -1;
				desc_set[i].events = 0;
			}
		}
	}
}

//elimina table
table_skel_destroy();

close(connsock);
close(listening_socket);

return 0;
}




