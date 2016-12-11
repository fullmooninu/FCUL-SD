/* Sistemas Distribuidos - 2016 - Grupo 34
Elias Miguel Barreira 40821, Pedro Pais 41375
Silvia Ferreira 45511 */

#include "primary_backup-private.h"
#include "table-private.h"
#include "network_client.h"
#include "table_skel.h"
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

/* Função usada para um servidor avisar o servidor “server” de que
*  já acordou. Retorna 0 em caso de sucesso, -1 em caso de insucesso
*/
int hello(struct server_t* server) {
	return 0;
	// manda mensagem a dizer que acordou com a informacao de si proprio
	// recebe acknoweledgement de ok
}

/* Pede atualizacao de estado ao server.
*  Retorna 0 em caso de sucesso e -1 em caso de insucesso.
*/
int update_state(struct server_t* server) {
	//send message with request code for table keys and data from primary
	// o primario tem de iterar a lista e fazer um put para cada par key data
		// secundario pede lista toda
		// primario manda mensagem com size actual
		// primario faz put para cada par key data
		// primario pede size == size_actual_secundario
		// primario manda update complete
	return 0;
}

int send_table(struct table_t *table, struct server_t* backup) {
	char** table_keys = table_get_keys(table);
	for (int i = 0; table_keys[i] != NULL; i++) {
		struct data_t* data = NULL;
		// prepare data
		data = table_get(table, table_keys[i]);
		// char* char_data = (char*) data->data; //TODO REMOVE THIS?

		// prepare message
		// TODO check if can use send key instead
		struct message_t* msg = NULL;
		msg = malloc(sizeof(struct message_t)); if (msg == NULL) return -1;
		msg->opcode=50;
		msg->c_type=30;
		msg->content.key = table_keys[i];
		msg->content.data = data;
		network_send_receive(backup,msg);
		data_destroy(data);
		free_message(msg);
	}
	return 0;
}

void printCurrentTable(struct table_t* table) {
	char input[81], *s;
	fgets(input, 80, stdin);
	s = strchr(input, '\n');
	*s = '\0';
	if (strcmp(input, "print") == 0) {
      printf("\n\n %p \n\n",table_get_keys(table));
    }
}
