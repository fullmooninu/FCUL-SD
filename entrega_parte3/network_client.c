/* Sistemas Distribuidos - 2016 - Grupo 34
 Elias Miguel Barreira 40821, Pedro Pais 41375
 Silvia Ferreira 45511 */

#include "network_client-private.h"
#include <stdlib.h>

struct server_t *network_connect(const char *address_port) {

	struct server_t *server = (struct server_t*) malloc(sizeof(struct server_t));
	char *server_address;
	char *server_port;

	/* Verificar parâmetro da função e alocação de memória */
	if (server == NULL || address_port == NULL) {
		network_close(server);
		return NULL;
	}

	server_address = strtok((char*) address_port, ":");

	if (server_address == NULL) {
		network_close(server);
		return NULL;
	}

	server->addrIP = atoi(server_address);
	printf("hostA: %s\n", server_address);

	server_port = strtok(NULL, ":");
	if (server_port == NULL) {
		network_close(server);
		return NULL;
	}
	server->port = atoi(server_port);
	printf("portA: %d\n", server->port);

	/* Estabelecer ligação ao servidor:

	 Preencher estrutura struct sockaddr_in com dados do
	 endereço do servidor.

	 Criar a socket.

	 Estabelecer ligação.
	 */

	//socket
	if ((server->socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Erro a criar socket TCP");
		network_close(server);
		return NULL;
	}

	server->addr->sin_family = AF_INET;
	server->addr->sin_port = htonl(server->port);

	//	hostinfo = gethostbyname(server_name);
	//	if (hostinfo == NULL) {
	//		network_close(server);
	//		return NULL;
	//	}
	//	server->addr.sin_addr = *(struct in_addr *) hostinfo->h_addr;


	if (inet_pton(AF_INET, server_address, &server->addr->sin_addr) < 1) {
		perror("Erro ao converter o IP\n");
		network_close(server);
		return NULL;
	}
//
//	/* Se a ligação não foi estabelecida, retornar NULL */
//	if (connect(server->socket_fd, (struct sockaddr *) &server->addr.sin_addr,
//			sizeof(server->addr.sin_addr)) < 0) {
//		perror("Sem ligação ao servidor");
//		printf("Falhou aqui!!!!!!\n\n");
//		network_close(server);
//		return NULL;
//	}
//


	 if (connect(server->socket_fd, (struct sockaddr *) server->addr, sizeof(struct sockaddr_in)) < 0){
		 printf("BURRO! ENtrou aqui");
		 network_close(server);
		 return NULL;
	 }
//
//		/* Se a ligação não foi estabelecida, retornar NULL */
//		if (connect(server->socket_fd, (struct sockaddr *) &server_in,
//				sizeof(server_in)) < 0) {
//			network_close(server);
//			return NULL;
////	}

	//	free(server_address);
	//	free(server_port);
	return server;

//	struct server_t *server = malloc(sizeof(struct server_t));
//	struct sockaddr_in server_in;
//	struct hostent *hostinfo;
//	char str_aux[1000];
//	char *server_name;
//	char *server_port;
//	int server_port_num = -1;
//	int server_socket_fd = -1;
//
//	/* Verificar parâmetro da função e alocação de memória */
//	if (server == NULL || address_port == NULL) {
//		network_close(server);
//		return NULL;
//	}
//
//	strcpy(str_aux, address_port);
//	server_name = strtok(str_aux, ":");
//	if (server_name == NULL) {
//		network_close(server);
//		return NULL;
//	}
//
//	printf("host: %s\n", server_name);
//	server_port = strtok(NULL, ":");
//	if (server_port == NULL) {
//		network_close(server);
//		return NULL;
//	}
//	server_port_num = atoi(server_port);
//	printf("port: %d\n", server_port_num);
//
//	/* Estabelecer ligação ao servidor:
//
//	 Preencher estrutura struct sockaddr_in com dados do
//	 endereço do servidor.
//
//	 Criar a socket.
//
//	 Estabelecer ligação.
//	 */
//
//	//socket
//	server_socket_fd = socket(PF_INET, SOCK_STREAM, 0);
//	if (server_socket_fd < 0) {
//		network_close(server);
//		return NULL;
//	}
//	server->socket_fd = server_socket_fd;
//
//	server_in.sin_family = AF_INET;
//	server_in.sin_port = htons(server_port_num);
//	hostinfo = gethostbyname(server_name);
//	if (hostinfo == NULL) {
//		network_close(server);
//		return NULL;
//	}
//	server_in.sin_addr = *(struct in_addr *) hostinfo->h_addr;
//
	//	/* Se a ligação não foi estabelecida, retornar NULL */
	//	if (connect(server_socket_fd, (struct sockaddr *) &server_in,
	//			sizeof(server_in)) < 0) {
	//		network_close(server);
	//		return NULL;
//	}
//
//	//TOD guardar mais info na estrutura server?
//
//	return server;
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
	printf("host size: %d\n", host_size);
	printf("net size: %d\n", net_size);
	fflush(stdout);
	if ((result = write_all(server->socket_fd, (char*) &net_size, _INT)) != _INT) {
		perror("Erro no enviar o tamanho da mensagem.");
		network_close(server);
		return NULL;
	}
	printf("write_all bytes escritos: %d\n", result);
	fflush(stdout);
	/* Verificar se o envio teve sucesso */

	/* Enviar a mensagem que foi previamente serializada */
	result = write_all(server->socket_fd, message_out, host_size);
	printf("bytes enviados buffer: %d\n", result);
	fflush(stdout);
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
	message_result = malloc(sizeof(char) * host_size);

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
