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

struct message_t* process_entry_command(char* input) {
  struct message_t* msg_out;
  char *key, *data;
  struct data_t *datat;

  strtok(input, " "); //ignorar o "comando"
  key = strtok(NULL, " ");
  if (key == NULL) {
    printf("Comando com formato inválido.\n");
    return NULL;
  }
  printf("key: <%s>\n", key);
  data = strtok(NULL, "");//le ate ao fim da linha 'input'
  if (data == NULL) {
    printf("Comando com formato inválido.\n");
    return NULL;
  }
  printf("data <%s>\n", data);

  msg_out = (struct message_t *) malloc(sizeof(struct message_t));
  if (msg_out == NULL) return NULL;

  msg_out->c_type = CT_ENTRY;
  msg_out->content.key = strdup(key);
  if (msg_out->content.key == NULL) {
    free_message(msg_out);
    return NULL;
  }

  printf("STRLEN data: %lu\n", strlen(data));
  datat = data_create2(strlen(data), data);
  if (datat == NULL) {
    free_message(msg_out);
    return NULL;
  }
  msg_out->content.entry = entry_create(key, datat);
  if (msg_out->content.entry == NULL) {
    free_message(msg_out);
    return NULL;
  }

  return msg_out;
}


struct message_t* process_key_command(char* input) {
  struct message_t* msg_out;
  char *key;

  strtok(input, " "); //ignorar o "comando"
  key = strtok(NULL, "");//le ate ao fim da linha 'input'
  if (key == NULL) {
    printf("Comando com formato inválido.\n");
    return NULL;
  }
  printf("key: <%s>\n", key);

  msg_out = (struct message_t *) malloc(sizeof(struct message_t));
  if (msg_out == NULL) return NULL;

  msg_out->c_type = CT_KEY;
  msg_out->content.key = strdup(key);
  if (msg_out->content.key == NULL) {
    free_message(msg_out);
    return NULL;
  }

  return msg_out;
}

int main(int argc, char **argv){
	struct server_t *server;
	struct message_t *msg_out = NULL, *msg_resposta = NULL;

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
    char input[81], *s;
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
      printf("Comando PUT\n");

      msg_out = process_entry_command(input);
      if (msg_out == NULL) return network_close(server);

      msg_out->opcode = OC_PUT;

    } else if (strncmp(input, "get ", 4) == 0) {
      printf("Comando GET\n");

      msg_out = process_key_command(input);
      if (msg_out == NULL) return network_close(server);

      msg_out->opcode = OC_GET;

    } else if (strncmp(input, "update ", 7) == 0) {
      printf("Comando UPDATE\n");

      msg_out = process_entry_command(input);
      if (msg_out == NULL) return network_close(server);

      msg_out->opcode = OC_UPDATE;

    } else if (strncmp(input, "del ", 4) == 0) {
      printf("Comando DEL\n");

      msg_out = process_key_command(input);
      if (msg_out == NULL) return network_close(server);

      msg_out->opcode = OC_DEL;

    } else if (strncmp(input, "size", 4) == 0) {
      printf("Comando SIZE\n");

      msg_out = (struct message_t *) malloc(sizeof(struct message_t));
      if (msg_out == NULL) return -1;

      msg_out->opcode = OC_SIZE;
      msg_out->c_type = -1;
    }

    if (msg_out != NULL) {
      printf("PEDIDO:\n");
      print_msg(msg_out);
      msg_resposta = network_send_receive(server, msg_out);

      printf("RESPOSTA:\n");
      print_msg(msg_resposta);
    }

    free_message(msg_out);
    free_message(msg_resposta);
	}//while
  	return network_close(server);
}
