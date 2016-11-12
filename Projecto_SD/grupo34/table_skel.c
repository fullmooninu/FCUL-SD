/* Sistemas Distribuidos - 2016 - Grupo 34
 Elias Miguel Barreira 40821, Pedro Pais 41375
 Silvia Ferreira 45511 */

#include <errno.h>
#include "table_skel.h"
#include "inet.h"
#include "table-private.h"
#include "table.h"

struct table_t *table;

int table_skel_init(int n_lists) {
	table = table_create(n_lists);

	if (table == NULL || table == errno)
		return -1;
	else
		return 0;

}

int table_skel_destroy() {
	table_destroy(table);
}

struct message_t *invoke(struct message_t *msg_in) {
	int aux;

	if (msg_in == NULL)
		return -1;

	switch (msg_in->opcode) {
	case OC_SIZE {
		aux = table_size(table);
		msg_in->content.result = aux;
		msg_in->opcode = OC_SIZE + 1;
		msg_in->c_type = CT_RESULT;
	}break;

	case OC_DEL {
		aux = table_del(table, msg_in->content.key);
		msg_in->opcode = OC_DEL + 1;
		msg_in->c_type = CT_RESULT;

		if((msg_in->content.result = aux)!= 0) return -1;

	}break;

	case OC_UPDATE {
		aux = table_update(table, msg_in->content.key, msg_in->content.value);
		msg_in->opcode = OC_UPDATE + 1;
		msg_in->c_type = CT_RESULT;

		if((msg_in->content.result = aux) != 0) return -1;
	}break;

	case OC_GET {

		msg_in->opcode = OC_GET + 1;
		msg_in->c_type = CT_VALUE;
		msg_in-> content.value = table_get(table, msg->content.key);

		//cria mensagem de resposta
		if(msg_in->content.value == NULL)
		msg_in->content.value= datacreate2(0,NULL);

	}break;

	case OC_PUT {

		msg_in->opcode = OC_PUT + 1;
		msg_in->c_type = CT_RESULT;
		aux = table_put(table, msg_in->content.entry->key, msg_in->content.entry->value);

		if((msg_in->content.result = aux) != 0) return -1;
	}break;

	default {
		printf("Mensagem recebida com OP_CODE inválido\n");
		return -1;
	}
	break;
}
return msg_in;
}
