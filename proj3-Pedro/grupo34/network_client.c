/* Sistemas Distribuidos - 2016 - Grupo 34
 Elias Miguel Barreira 40821, Pedro Pais 41375
 Silvia Ferreira 45511 */

#include "network_client-private.h"
#include <stdlib.h>

struct server_t *network_connect(const char *address_port) {
	struct server_t *server = malloc(sizeof(struct server_t));

		struct hostent *hostinfo;
		char str_aux[1000];
//		int server_port_num = -1;
//		int server_socket_fd = -1;

		/* Verificar parâmetro da função e alocação de memória */
		if (server == NULL || address_port == NULL) {
			network_close(server);
			return NULL;
		}

		strcpy(str_aux, address_port);
		server->IP = strtok(str_aux, ":");
		if (server->IP == NULL) {
			perror("Imposs�vel obter IP");
			network_close(server);
			return NULL;
		}

		// printf("host: %s\n", server_name);
		server->port = atoi(strtok(NULL, ":"));
		if (server->port < 0) {
			perror("Erro ao obter o porto do servidor! \n");
			network_close(server);
			return NULL;
		}
//		server_port_num = server->server_port;
		// printf("port: %d\n", server_port_num);

		/* Estabelecer ligação ao servidor:

		 Preencher estrutura struct sockaddr_in com dados do
		 endereço do servidor.

		 Criar a socket.

		 Estabelecer ligação.
		 */

		//socket
		server->socket_fd = socket(PF_INET, SOCK_STREAM, 0);
		if (server->socket_fd < 0) {
			network_close(server);
			return NULL;
		}

		server->server_in.sin_family = AF_INET;
		server->server_in.sin_port = htons(server->port);
		hostinfo = gethostbyname(server->IP);
		if (hostinfo == NULL) {
			network_close(server);
			return NULL;
		}
		server->server_in.sin_addr = *(struct in_addr *)hostinfo->h_addr;

		/* Se a ligação não foi estabelecida, retornar NULL */
		if (connect(server->socket_fd, (struct sockaddr *) &server->server_in,
				sizeof(server->server_in)) < 0) {
			network_close(server);
			return NULL;
		}


	//TOD guardar mais info na estrutura server?

	return server;
}

struct message_t *network_send_receive(struct server_t *server,
		struct message_t *msg) {
	char *message_out;
	int host_size, net_size, result;
	struct message_t *msg_resposta;
	//int nBytes;
	char *message_result = NULL; //

	/* Verificar parâmetros de entrada */
	if (server == NULL || msg == NULL)
		return NULL;

	/* Serializar a mensagem recebida */
	host_size = message_to_buffer(msg, &message_out);

	/* Verificar se a serialização teve sucesso */
	net_size = htonl(host_size);

	/* Enviar ao servidor o tamanho da mensagem que será enviada
	 logo de seguida
	 */
	// printf("host size: %d\n", host_size);
	// printf("net size: %d\n", net_size);
	fflush(stdout);
	if ((result = write_all(server->socket_fd, (char*) &net_size, _INT)) != _INT) {
		perror("Erro no enviar o tamanho da mensagem.");
		network_close(server);
		return NULL;
	}
	// printf("write_all bytes escritos: %d\n", result);
	// fflush(stdout);
	/* Verificar se o envio teve sucesso */


	/* Enviar a mensagem que foi previamente serializada */
	result = write_all(server->socket_fd, message_out, host_size);
	// printf("bytes enviados buffer: %d\n", result);
	// fflush(stdout);
	/* Verificar se o envio teve sucesso */

	/* De seguida vamos receber a resposta do servidor:

	 Com a função read_all, receber num inteiro o tamanho da
	 mensagem de resposta.
	*/
result = read_all(server->socket_fd, (char *) &net_size, _INT);
//TODO validar result
host_size = ntohl(net_size);
/*
	 Alocar memória para receber o número de bytes da
	 mensagem de resposta.

	 Com a função read_all, receber a mensagem de resposta.

	 */
	 message_result = malloc(sizeof(char)*host_size);

	 result = read_all(server->socket_fd, message_result, host_size);

	/* Desserializar a mensagem de resposta */
	msg_resposta = buffer_to_message(message_result, net_size);
	//msg_resposta = buffer_to_message( /* */ );
	//printf("antes do read");
	//fflush(stdout);
	// if ((nBytes = read(server->socket_fd, &result, sizeof(result)))
	// 		!= sizeof(result)) {
	// 	perror("Erro ao receber dados do servidor");
	// 	network_close(server);
	// 	return NULL;
	// }
	//printf("depois do read");
	//fflush(stdout);

	// int size = ntohl(result);
	// message_result = (char *)malloc(size + 1);
	//
	// /* Verificar se a desserialização teve sucesso */
	// //printf("antes do read_all");
	// if ((nBytes = read_all(server->socket_fd, message_result, net_size))
	// 		!= net_size) {
	// 	perror("Erro no receber os dados do servidor");
	// 	network_close(server);
	// 	return NULL;
	// }
	// //printf("depoiss do read_all");


	/* Libertar memória */
	free(message_out);
	free(message_result);

	return msg_resposta;
}

int network_close(struct server_t *server) {
	/* Verificar parâmetros de entrada */
	if (server == NULL)
		return -1;

	/* Terminar ligação ao servidor */
	close(server->socket_fd);

	/* Libertar memória */
	free(server);
	//TOD ver se eh preciso libertar memória
	// dentro da struct 'server'

	return 0;
}
