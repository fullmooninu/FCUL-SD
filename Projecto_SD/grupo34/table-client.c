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

#include "client_stub-private.h"
#include "client_stub.h"
#include <string.h>
#include "network_client.h"
#include "message.h"
#include <unistd.h>
#include <stdlib.h>

//
//struct message_t* process_entry_command(char* input) {
//  struct message_t* msg_out;
//  char *key, *data;
//  struct data_t *datat;
//
//  strtok(input, " "); //ignorar o "comando"
//  key = strtok(NULL, " ");
//  if (key == NULL) {
//    printf("Comando com formato inválido.\n");
//    return NULL;
//  }
//  printf("key: <%s>\n", key);
//  data = strtok(NULL, "");//le ate ao fim da linha 'input'
//  if (data == NULL) {
//    printf("Comando com formato inválido.\n");
//    return NULL;
//  }
//  printf("data <%s>\n", data);
//
//  msg_out = (struct message_t *) malloc(sizeof(struct message_t));
//  if (msg_out == NULL) return NULL;
//
//  msg_out->c_type = CT_ENTRY;
//  msg_out->content.key = strdup(key);
//  if (msg_out->content.key == NULL) {
//    free_message(msg_out);
//    return NULL;
//  }
//  printf("STRLEN data: %d\n", strlen(data));
//  datat = data_create2(strlen(data), data);
//  if (datat == NULL) {
//    free_message(msg_out);
//    return NULL;
//  }
//  msg_out->content.entry = entry_create(key, datat);
//  if (msg_out->content.entry == NULL) {
//    free_message(msg_out);
//    return NULL;
//  }
//
//  return msg_out;
//}
//
//
//struct message_t* process_key_command(char* input) {
//  struct message_t* msg_out;
//  char *key, *data;
//
//  strtok(input, " "); //ignorar o "comando"
//  key = strtok(NULL, "");//le ate ao fim da linha 'input'
//  if (key == NULL) {
//    printf("Comando com formato inválido.\n");
//    return NULL;
//  }
//  printf("key: <%s>\n", key);
//
//  msg_out = (struct message_t *) malloc(sizeof(struct message_t));
//  if (msg_out == NULL) return NULL;
//
//  msg_out->c_type = CT_KEY;
//  msg_out->content.key = strdup(key);
//  if (msg_out->content.key == NULL) {
//    free_message(msg_out);
//    return NULL;
//  }
//
//  return msg_out;
//}

int main(int argc, char **argv) {
	struct rtable_t *server = (struct rtable_t*) malloc(
			sizeof(struct rtable_t));
	char input[81], *s;
	//struct message_t *msg_out = NULL, *msg_resposta = NULL;

	/* Testar os argumentos de entrada */
	if (argc != 2) {
		printf("Número de argumentos inválido.\n");
		return -1;
	}

	/*--------------------------------------------------------------*/

	/* Fazer a ligação ao servidor */

	char address[100];

	/* Formar a string no formato <ip servidor>:<porta servidor> */
	strcpy(address, argv[1]);
	strcat(address, ":");
	strcat(address, argv[2]);
	strcat(address, "\0");

	server = rtable_bind(address);

	if (server == NULL) {
		printf("Erro ao estabelecer ligação ao servidor: %s\n", argv[1]);
		return -1;
	}

	printf("Escolha um dos comandos abaixo:\n\n");
	printf("put\n get\n update\n delete\n size\n get keys\n quit\n\n");
	// Mostrar a prompt para inserção de comando

	/* Receber o comando introduzido pelo utilizador
	 Sugestão: usar fgets de stdio.h
	 Quando pressionamos enter para finalizar a entrada no
	 comando fgets, o carater \n é incluido antes do \0.
	 Convém retirar o \n substituindo-o por \0.
	 */
	fgets(input, 80, stdin);
	s = strchr(input, '\n');
	*s = '\0';

	/* Fazer ciclo até que o utilizador resolva fazer "quit" */
	while (strncmp(input, "quit",4) != 0) {

		char *key = (char*) malloc(sizeof(char) * MAX_MSG);
		char *data = (void *) malloc(sizeof(void*) * MAX_MSG);
		char *aux = (char *) malloc(sizeof(char));
		int int_aux;

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

		if (strncmp(input, "put ",4) == 0 ) {

			fgets(aux, MAX_MSG, stdin);
			printf("Key: ");
			fflush(stdin);
			fgets(key, MAX_MSG, stdin);
			printf("Data: ");
			fflush(stdin);
			fgets(data, MAX_MSG, stdin);
			struct data_t *dataN = data_create2(strlen(data), data);
			int_aux = rtable_put(server, key, dataN);
			if (int_aux == -1) {
				printf(
				"Os dados não foram introduzidos com sucesso!\n");
			} else
			printf("Os dados foram introduzidos com sucesso!\n");

			free(aux);
			free(dataN);
			free(key);
			free(data);
		}
		//

		else if (strncmp(input, "get ",4) == 0) {
			printf("Comando GET\n");

			fgets(aux, MAX_MSG, stdin);
			printf("Key: ");
			fflush(stdin);
			fgets(key, MAX_MSG, stdin);
			if (rtable_get(server,key) == NULL) {
				printf("Os dados não existem na tabela");
			} else
			printf("Foi encontrado: %s\n", (char*) rtable_get(server, key));

			free(aux);
			free(key);

//      msg_out = process_key_command(input);
//      if (msg_out == NULL) return network_close(server);
//
//      msg_out->opcode = OC_GET;

		}

		else if (strncmp(input, "update ", 7) == 0) {
			printf("Comando UPDATE\n");

			//TODO UPDATE
//      msg_out = process_entry_command(input);
//      if (msg_out == NULL) return network_close(server);
//
//      msg_out->opcode = OC_UPDATE;

		}

		else if (strncmp (input, "delete ",7) == 0) {
			printf("Comando DEL\n");

			fgets(aux, MAX_MSG, stdin);
			printf("Key: ");
			fflush(stdin);
			fgets(key, MAX_MSG, stdin);
			int_aux = rtable_del(server, key);
			if (int_aux == -1) {
				printf(
				"Não foi possível remover a entrada na tabela\n");
			} else
			printf("Os dados foram removidos\n");

			free(aux);
			free(key);

//      msg_out = process_key_command(input);
//      if (msg_out == NULL) return network_close(server);
//
//      msg_out->opcode = OC_DEL;

		}

		else if (strncmp(input, "size ", 5) == 0) {
			printf("Comando SIZE\n");
			int_aux = rtable_size(server);
			if (int_aux == -1) {
				printf(
				"Não foi possível determinar o número de elementos na tabela!\n");
			} else
			printf("A tabela tem %d elementos!\n",
			rtable_size(server));

//      msg_out = (struct message_t *) malloc(sizeof(struct message_t));
//      if (msg_out == NULL) return NULL;
//
//      msg_out->opcode = OC_SIZE;
//      msg_out->c_type = NULL;
		}

		else if (strncmp(input, "get keys ", 9) == 0) {
			if ((rtable_get_keys(server)) == NULL) {
				printf("Não se conseguiu obter as chaves!\n");
			} else {
				for (int i = 0; rtable_get_keys(server)[i] != NULL;
				i++) {
					printf("Key -> %s;",
					(char*) rtable_get_keys(server)[i]);
				}
			}

		}

//			if (msg_out != NULL) {
//				printf("PEDIDO:\n");
//				print_msg(msg_out);
//				msg_resposta = network_send_receive(server, msg_out);
//
//				printf("RESPOSTA:\n");
//				print_msg(msg_resposta);
//			}
//
//			free_message(msg_out);
//			free_message(msg_resposta);
	} //while

	if (strncmp(input, "quit", 4) == 0)
		rtable_unbind(server);

	return 0;
}
