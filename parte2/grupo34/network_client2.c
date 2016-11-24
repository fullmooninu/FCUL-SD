/* Sistemas Distribuidos - 2016 - Grupo 34
 Elias Miguel Barreira 40821, Pedro Pais 41375
 Silvia Ferreira 45511 */

#include "network_client-private2.h"
#include <stdlib.h>
#include <netdb.h> // TODO donde veio este modulo

//int write_all(int sock, char *buf, int len){
//}
// TODO porque estah definido no message?

//int read_all(int sock, char *buf, int len){
//}
// TODO porque estah definido no message?


struct server_t *network_connect(const char *address_port) {
	/* Verificar parâmetro da função e alocação de memória */
	if (address_port == NULL) return NULL;
	struct server_t *server = malloc(sizeof(struct server_t));
	if (server == NULL) return NULL;

	/* Estabelecer ligação ao servidor:

		Preencher estrutura struct sockaddr_in com dados do
		endereço do servidor.

		Criar a socket.

		Estabelecer ligação.
	*/

	char* server_name = strtok((char*) address_port, ":");
	char* server_port_str = strtok(NULL, ":");
	if (server_name == NULL || server_port_str == NULL) {
		network_close(server);
		return NULL;
	}
	int server_port = atoi(server_port_str);
	printf("host: %s\n", server_name);
	printf("port: %d\n", server_port);

	// SOCKET THINGS
	int server_socket_fd = -1;
	server_socket_fd = socket(PF_INET, SOCK_STREAM, 0);
	if (server_socket_fd < 0) {
		network_close(server);
		return NULL;
	}
	server->socket_fd = server_socket_fd;

	struct sockaddr_in server_in;
	server_in.sin_family = AF_INET;
	server_in.sin_port = htons(server_port);

	struct hostent *hostinfo;
	hostinfo = gethostbyname(server_name);
	if (hostinfo == NULL) {
		network_close(server);
		return NULL;
	}
	server_in.sin_addr = *(struct in_addr *) hostinfo->h_addr;

	/* Se a ligação não foi estabelecida, retornar NULL */
	if (connect(server_socket_fd, (struct sockaddr *) &server_in,
		sizeof(server_in)) < 0) {
		network_close(server);
	return NULL;
}

free(server);
return server;
}

struct message_t *network_send_receive(struct server_t *server, struct message_t *msg){
	char *message_out;
	int message_size, msg_size, result;
	struct message_t* msg_resposta;

	/* Verificar parâmetros de entrada */
	if (server == NULL || msg == NULL) return NULL;

	/* Serializar a mensagem recebida */
	message_size = message_to_buffer(msg, &message_out);

	/* Verificar se a serialização teve sucesso */
	if (message_size == -1) return NULL;

	/* Enviar ao servidor o tamanho da mensagem que será enviada
	   logo de seguida
	*/
	msg_size = htonl(message_size);
	result = write_all(server->socket_fd, (char*) &msg_size, _INT);

	/* Verificar se o envio teve sucesso */
	if (result != _INT) return NULL;

	/* Enviar a mensagem que foi previamente serializada */
	result = write_all(server->socket_fd, message_out, message_size);

	/* Verificar se o envio teve sucesso */
	if (result != message_size) return NULL;

	/* De seguida vamos receber a resposta do servidor:

		Com a função read_all, receber num inteiro o tamanho da 
		mensagem de resposta.

		Alocar memória para receber o número de bytes da
		mensagem de resposta.

		Com a função read_all, receber a mensagem de resposta.
		
	*/
	
	result = read_all(server->socket_fd, (char *) &msg_size, _INT);
	// TODO como verificar se o read_all falhou?
	message_size = ntohl(msg_size);
	
	char* msg_resposta_rede = NULL;
	msg_resposta_rede = malloc(sizeof(char)*message_size);
	if (msg_resposta_rede == NULL) return NULL;

	result = read_all(server->socket_fd, msg_resposta_rede, message_size);
	if (result != message_size) return NULL;


	/* Desserializar a mensagem de resposta */
	msg_resposta = buffer_to_message( msg_resposta_rede, result );
	if (msg_resposta == NULL) return NULL;

	/* Verificar se a desserialização teve sucesso */

	/* Libertar memória */
	free(message_out);
	free(msg_resposta_rede);
	return msg_resposta;
}

int network_close(struct server_t *server){
	/* Verificar parâmetros de entrada */
	if (server == NULL) return -1;
	/* Terminar ligação ao servidor */
	close(server -> socket_fd);
	/* Libertar memória */
	free(server);
	return 0;
}

