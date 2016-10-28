/* Sistemas Distribuidos - 2016 - Grupo 34
Elias Miguel Barreira 40821, Pedro Pais 41375
Silvia Ferreira 45511 */

#include "network_client-private.h"

#include <stdlib.h>


int write_all(int sock, char *buf, int len){
}


int read_all(int sock, char *buf, int len){
}


struct server_t *network_connect(const char *address_port){
	struct server_t *server = malloc(sizeof(struct server_t));

	/* Verificar parâmetro da função e alocação de memória */
	if(address_port == NULL) return NULL;

	int sockfd;


	/* Estabelecer ligação ao servidor:

		Preencher estrutura struct sockaddr_in com dados do
		endereço do servidor.

		Criar a socket.

		Estabelecer ligação.
	*/
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Erro ao criar o socket");
		return -1;
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(/**qual porta que se coloca aqui??**/);
	server.sin_addr.s_addr = inet_addr(/**qual o address que se coloca aqui??**/);

	/* Se a ligação não foi estabelecida, retornar NULL */

	if(connect(sockfd,(struct server_t *)&server,sizeof(server))<0)
	{
		return NULL;
	}

	return server;
}

struct message_t *network_send_receive(struct server_t *server, struct message_t *msg){
	char *message_out;
	int message_size, msg_size, result;
	struct message_t msg_resposta;

	/* Verificar parâmetros de entrada */
	if (server == NULL || msg == NULL) return NULL;

	/* Serializar a mensagem recebida */
	message_size = message_to_buffer(msg, &message_out));

	/* Verificar se a serialização teve sucesso */

	/* Enviar ao servidor o tamanho da mensagem que será enviada
	   logo de seguida
	*/
	msg_size = htonl(message_size);
 	result = write_all(server->/*atributo*/, (char *) &msg_size, _INT));

	/* Verificar se o envio teve sucesso */

	/* Enviar a mensagem que foi previamente serializada */

	result = write_all(server->/*atributo*/, message_out, message_size));

	/* Verificar se o envio teve sucesso */

	/* De seguida vamos receber a resposta do servidor:

		Com a função read_all, receber num inteiro o tamanho da 
		mensagem de resposta.

		Alocar memória para receber o número de bytes da
		mensagem de resposta.

		Com a função read_all, receber a mensagem de resposta.
		
	*/

	/* Desserializar a mensagem de resposta */
	msg_resposta = buffer_to_message( /* */, /* */ );

	/* Verificar se a desserialização teve sucesso */

	/* Libertar memória */

	return msg_resposta;
}

int network_close(struct server_t *server){
	/* Verificar parâmetros de entrada */
	if(server == NULL) return NULL;
	/* Terminar ligação ao servidor */
	close(server);
	/* Libertar memória */
	free(server);
}

