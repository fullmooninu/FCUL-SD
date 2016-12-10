/* Sistemas Distribuidos - 2016 - Grupo 34
 Elias Miguel Barreira 40821, Pedro Pais 41375
 Silvia Ferreira 45511 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "client_stub.h"
#include "client_stub-private.h"
#include "network_client-private.h"

//
struct rtable_t *rtable_bind(const char *address_port) {
	struct rtable_t *rtable = (struct rtable_t*) malloc(
			sizeof(struct rtable_t));

	if (rtable == NULL) {
		return NULL;
	}

	rtable->server = network_connect(address_port);

	if (rtable->server == NULL) {
		return NULL;
	}

	return rtable;
}

int rtable_unbind(struct rtable_t *rtable) {
	if (network_close(rtable->server) == -1)
		return -1;
	free(rtable);
	return 0;
}

int rtable_put(struct rtable_t *rtable, char *key, struct data_t *data) {
	struct message_t *msg_out, *msg_resposta;
	struct data_t *datat = (struct data_t *) malloc(sizeof(struct data_t));
	int result = -1;

	msg_out = (struct message_t *) malloc(sizeof(struct message_t));

	if (msg_out == NULL)
		return -1;

	msg_out->c_type = CT_ENTRY;

	datat = data_create2(data->datasize, data->data);
	if (datat == NULL) {
		free_message(msg_out);
		free(datat);
		return -1;
	}
	msg_out->content.entry = entry_create(key, datat);
	if (msg_out->content.entry == NULL) {
		data_destroy(datat);
		free_message(msg_out);
		free(datat);
		return -1;
	}

	msg_out->opcode = OC_PUT;

	msg_resposta = network_send_receive(rtable->server, msg_out);

	if (msg_resposta->opcode == OC_RT_ERROR) {
		//Tolerância a falta definida no enunciado Projeto 3
		sleep(RETRY_TIME);
		msg_resposta = network_send_receive(rtable->server, msg_out);
		if (msg_resposta->opcode == OC_RT_ERROR)
			return -1;
	} else if (msg_resposta->opcode == OC_PUT + 1&&
	msg_resposta->c_type == CT_RESULT) {
		result = msg_resposta->content.result;
	}

	data_destroy(datat);
	free_message(msg_out);
	free_message(msg_resposta);
	free(datat);

	return result;
}

int rtable_update(struct rtable_t *rtable, char *key, struct data_t *data) {
	struct message_t *msg_out, *msg_resposta;
	struct data_t *datat = (struct data_t *) malloc(sizeof(struct data_t));
	int result = -1;

	msg_out = (struct message_t *) malloc(sizeof(struct message_t));
	if (msg_out == NULL)
		return -1;

	msg_out->c_type = CT_ENTRY;

	datat = data_create2(data->datasize, data->data);
	if (datat == NULL) {
		free_message(msg_out);
		free(datat);
		return -1;
	}
	msg_out->content.entry = entry_create(key, datat);
	if (msg_out->content.entry == NULL) {
		data_destroy(datat);
		free_message(msg_out);
		free(datat);
		return -1;
	}

	msg_out->opcode = OC_UPDATE;

	msg_resposta = network_send_receive(rtable->server, msg_out);

	if (msg_resposta->opcode == OC_RT_ERROR) {
		//Tolerância a falta definida no enunciado Projeto 3
		sleep(RETRY_TIME);
		msg_resposta = network_send_receive(rtable->server, msg_out);
		if (msg_resposta->opcode == OC_RT_ERROR)
			result = -1;
	} else if (msg_resposta->opcode == OC_UPDATE + 1&&
	msg_resposta->c_type == CT_RESULT) {
		result = msg_resposta->content.result;
	}

	data_destroy(datat);
	free_message(msg_out);
	free_message(msg_resposta);
	free(datat);

	return result;
}

struct data_t *rtable_get(struct rtable_t *table, char *key) {
	struct message_t *msg_out, *msg_resposta;
	struct data_t *datat = (struct data_t *) malloc(sizeof(struct data_t));

	msg_out = (struct message_t *) malloc(sizeof(struct message_t));
	if (msg_out == NULL)
		return NULL;

	msg_out->c_type = CT_KEY;
	msg_out->content.key = strdup(key);
	if (msg_out->content.key == NULL) {
		free_message(msg_out);
		free(datat);
		return NULL;
	}

	msg_out->opcode = OC_GET;

	msg_resposta = network_send_receive(table->server, msg_out);

	if (msg_resposta->opcode == OC_RT_ERROR) {
		//Tolerância a falta definida no enunciado Projeto 3
		sleep(RETRY_TIME);
		msg_resposta = network_send_receive(table->server, msg_out);
		if (msg_resposta->opcode == OC_RT_ERROR)
			datat = NULL;
	} else if (msg_resposta->opcode == OC_GET + 1&&
	msg_resposta->c_type == CT_VALUE) {

		if (msg_resposta->content.data->datasize == 0) {
			datat = (struct data_t*) malloc(sizeof(struct data_t));
			if (datat != NULL) {
				datat->datasize = 0;
				datat->data = NULL;
			}
		} else {

			// print_data(msg_resposta->content.data);
			datat = data_dup(msg_resposta->content.data);
		}
	}

	free_message(msg_out);
	free_message(msg_resposta);

	return datat;
}

int rtable_del(struct rtable_t *table, char *key) {
	struct message_t *msg_out, *msg_resposta;
	int result = -1;

	msg_out = (struct message_t *) malloc(sizeof(struct message_t));
	if (msg_out == NULL)
		return -1;

	msg_out->c_type = CT_KEY;
	msg_out->content.key = strdup(key);
	if (msg_out->content.key == NULL) {
		free_message(msg_out);
		return -1;
	}

	msg_out->opcode = OC_DEL;

	msg_resposta = network_send_receive(table->server, msg_out);

	if (msg_resposta->opcode == OC_RT_ERROR) {
		//Tolerância a falta definida no enunciado Projeto 3
		sleep(RETRY_TIME);
		msg_resposta = network_send_receive(table->server, msg_out);
		if (msg_resposta->opcode == OC_RT_ERROR)
			result = -1;
	} else if (msg_resposta->opcode == OC_DEL + 1&&
	msg_resposta->c_type == CT_RESULT) {
		result = msg_resposta->content.result;
	}

	free_message(msg_out);
	free_message(msg_resposta);

	return result;
}

int rtable_size(struct rtable_t *rtable) {
	struct message_t *msg_out, *msg_resposta;
	int result = -1;

	msg_out = (struct message_t *) malloc(sizeof(struct message_t));
	if (msg_out == NULL)
		return -1;

	msg_out->opcode = OC_SIZE;
	msg_out->c_type = -1;

	msg_resposta = network_send_receive(rtable->server, msg_out);

	if (msg_resposta->opcode == OC_RT_ERROR) {
		//Tolerância a falta definida no enunciado Projeto 3   ff
		sleep(RETRY_TIME);
		msg_resposta = network_send_receive(rtable->server, msg_out);
		if (msg_resposta->opcode == OC_RT_ERROR)
			result = -1;
	} else if (msg_resposta->opcode == OC_SIZE + 1&&
	msg_resposta->c_type == CT_RESULT) {
		result = msg_resposta->content.result;
	}

	free_message(msg_out);
	free_message(msg_resposta);

	return result;
}

char **rtable_get_keys(struct rtable_t *rtable) {
	struct message_t *msg_out, *msg_resposta;
	char **keys = NULL;
	int n_keys = 0;

	msg_out = (struct message_t *) malloc(sizeof(struct message_t));
	if (msg_out == NULL)
		return NULL;

	msg_out->c_type = CT_KEY;
	msg_out->content.key = strdup("*");

	msg_out->opcode = OC_GET;

	msg_resposta = network_send_receive(rtable->server, msg_out);

	if (msg_resposta->opcode == OC_RT_ERROR) {
		//Tolerância a falta definida no enunciado Projeto 3
		sleep(RETRY_TIME);
		msg_resposta = network_send_receive(rtable->server, msg_out);
		if (msg_resposta->opcode == OC_RT_ERROR)
			keys = NULL;
	} else if (msg_resposta->opcode == OC_GET + 1&&
	msg_resposta->c_type == CT_KEYS) {
		keys = msg_resposta->content.keys;
		//keys_size
		while (*keys != NULL) {
			keys++;
			n_keys++;
		}
		// printf("KEYS SIZE: %d\n", n_keys);

		keys = (char **) malloc((n_keys + 1) * sizeof(char*));

		for (int i = 0; i < n_keys; i++) {
			keys[i] = strdup(msg_resposta->content.keys[i]);
		}
		keys[n_keys] = NULL;

	}

	free_message(msg_out);
	free_message(msg_resposta);

	return keys;
}

void rtable_free_keys(char **keys) {
	if (keys != NULL) {
		for (int i = 0; keys[i] != NULL; i++) {
			free(keys[i]);
		}
		free(keys);
	}
}
