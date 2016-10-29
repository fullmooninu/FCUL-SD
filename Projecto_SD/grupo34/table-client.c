/* Sistemas Distribuidos - 2016 - Grupo 34
Elias Miguel Barreira 40821, Pedro Pais 41375
Silvia Ferreira 45511 */

/*
	Programa cliente para manipular tabela de hash remota.
	Os comandos introduzido no programa não deverão exceder
	80 carateres.

	Uso: table-client <ip servidor>:<porta servidor>
	Exemplo de uso: ./table_client 10.101.148.144:54321
*/

#include "network_client-private.h"

int main(int argc, char **argv){
	struct server_t *server;
	char input[81], *s;
	struct message_t *msg_out, *msg_resposta;

	/* Testar os argumentos de entrada */
  if (argc != 2) {
    printf("Número de argumentos inválido.\n");
    return -1;
  }

	/* Usar network_connect para estabelcer ligação ao servidor */
	server = network_connect(argv[1]);
  if (server == NULL) {
    printf("Erro ao estabelecer ligação ao servidor: %s\n", argv[1]);
    return -1;
  }

	/* Fazer ciclo até que o utilizador resolva fazer "quit" */
 	while (1){

		printf(">>> "); // Mostrar a prompt para inserção de comando

		/* Receber o comando introduzido pelo utilizador
		   Sugestão: usar fgets de stdio.h
		   Quando pressionamos enter para finalizar a entrada no
		   comando fgets, o carater \n é incluido antes do \0.
		   Convém retirar o \n substituindo-o por \0.
		*/
    fgets(input, 80, stdin);
    s = strchr(input, '\n');
    *s = '\0';
    // printf("Leu <%s>\n", input);

		/* Verificar se o comando foi "quit". Em caso afirmativo
		   não há mais nada a fazer a não ser terminar decentemente.
		 */
		/* Caso contrário:

			Verificar qual o comando;

			Preparar msg_out;

			Usar network_send_receive para enviar msg_out para
			o server e receber msg_resposta.
		*/

    if (strcmp(input, "quit") == 0) {
      return network_close(server);
    }

    if (strncmp(input, "put ", 4) == 0) {
      printf("Leu PUT\n");

    } else if (strncmp(input, "get ", 4) == 0) {
      printf("Leu GET\n");

    } else if (strncmp(input, "update ", 7) == 0) {
      printf("Leu UPDATE\n");

    } else if (strncmp(input, "del ", 4) == 0) {
      printf("Leu DEL\n");

    } else if (strncmp(input, "size", 4) == 0) {
      printf("Leu SIZE\n");

    }

	}
  	return network_close(server);
}
