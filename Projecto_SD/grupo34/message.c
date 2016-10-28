#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>

#include "message-private.h"

void free_message(struct message_t *msg) {
	if (msg == NULL) return;
	switch (msg->c_type) {
		case CT_VALUE:
			data_destroy(msg->content.data);
			break;
		case CT_KEY:
			free(msg->content.key);
			break;
		case CT_KEYS:
			list_free_keys(msg->content.keys);
			break;
		case CT_ENTRY:
			entry_destroy(msg->content.entry);
			break;
		default: ;
	}
}

int message_to_buffer(struct message_t *msg, char **msg_buf) {

	/* Verificar se msg é NULL */
	if (msg == NULL || msg_buf == NULL)
		return -1;

	/* Consoante o msg->c_type, determinar o tamanho do vetor de bytes
	 que tem de ser alocado antes de serializar msg
	 */
	int buffer_size; //Tamanho em bytes

	//Variavel auxiliar para o tamanho de cada chave (CT_KEYS)
	int keysSize;

	//REVER TAMANHOS
	switch (msg->c_type) {
	case CT_ENTRY:
		buffer_size = 6 + sizeof(msg->content.key) + 4;
		if ((msg->content.data->datasize) != 0)
			buffer_size += sizeof(msg->content.data->data);
		break;
	case CT_KEY:
		buffer_size = (6 + sizeof(msg->content.key));
		break;
	case CT_KEYS:
		for (int i = 0; i < sizeof(msg->content.keys) - 1; i++) {
			keysSize += 2 + strlen(msg->content.keys[i]);
		}
		buffer_size = 8 + keysSize;
		break;
	case CT_VALUE:
		buffer_size = 8;
		if ((msg->content.data->datasize) != 0)
			buffer_size += sizeof(msg->content.data->data);
		break;
	case CT_RESULT:
		buffer_size = 8;
		break;
	}

	/* Alocar quantidade de memória determinada antes */
	msg_buf = malloc(buffer_size);

	/* Inicializar ponteiro auxiliar com o endereço da memória alocada */
	char *ptr;
	ptr = *msg_buf;

	short short_value = htons(msg->opcode);
	memcpy(ptr, &short_value, _SHORT);
	ptr += _SHORT;

	short_value = htons(msg->c_type);
	memcpy(ptr, &short_value, _SHORT);
	ptr += _SHORT;

	/* Consoante o conteúdo da mensagem, continuar a serialização da mesma */

	int int_aux = 0;
	short short_aux = 0;

	//Vari�vel auxiliar para saber o numero de keys
	int nKeys = 0;

	switch (msg->c_type) {
	case CT_ENTRY:
		short_aux = htons(strlen(msg->content.key));
		memcpy(ptr, &short_aux, _SHORT);
		ptr += _SHORT;
		//colocar a chave
		char *key_aux;
		key_aux = (char *) malloc(sizeof(msg->content.key));
		strcpy(key_aux, msg->content.key);
		if (key_aux == NULL)
			return -1;
		memcpy(ptr, &key_aux, strlen(key_aux));
		if (ptr == NULL)
			return -1;
		ptr += strlen(key_aux); //j� sabemos que o strlen nao contabiliza o '/0'
		free(key_aux);
		//colocar o data_sizeDS
		int_aux = htonl(msg->content.data->datasize);
		memcpy(ptr, &int_aux, _INT);
		ptr += _INT;
		if ((msg->content.data->datasize) != 0) {
			//colocar o data->data
			void *key_aux;
			int_aux = msg->content.data->datasize;
			key_aux = malloc(int_aux);
			memcpy(ptr, &key_aux, int_aux);
			ptr += int_aux;
			free(key_aux);
		}
		break;
	case CT_KEY:
		short_aux = htons(strlen(msg->content.key));
		memcpy(ptr, &short_aux, _SHORT);
		ptr += _SHORT;
		//colocar a chave
		key_aux = (char *) malloc(sizeof(msg->content.key));
		strcpy(key_aux, msg->content.key);
		memcpy(ptr, &key_aux, strlen(key_aux));
		ptr += strlen(key_aux); //j� sabemos que o strlen nao contabiliza o '/0'
		free(key_aux);
		break;

	case CT_KEYS:
		key_aux = msg->content.keys[0];
		for (int i = 0; i < sizeof(msg->content.keys) - 1; i++) {
			nKeys++;
			key_aux += strlen(msg->content.keys[i]);
		}
		int_aux = htonl(nKeys);
		memcpy(ptr, &int_aux, _INT);
		ptr += _INT;
		//colocar as varias chaves
		for (int i = 0; i < sizeof(msg->content.keys) - 1; i++) {
			key_aux = (char *) malloc(sizeof(msg->content.keys[i]));
			strcpy(key_aux, msg->content.keys[i]);
			short_aux = htons(strlen(key_aux));
			memcpy(ptr, &short_aux, _SHORT);
			ptr += _SHORT;
			memcpy(ptr, &key_aux, strlen(key_aux));
			ptr += strlen(key_aux);
			free(key_aux);
		}
		break;

	case CT_VALUE:
		//colocar o data_sizeDS
		int_aux = htonl(msg->content.data->datasize);
		memcpy(ptr, &int_aux, _INT);
		ptr += _INT;
		if ((msg->content.data->datasize) != 0) {
			//colocar o data->data
			void *key_aux;
			int_aux = htonl(msg->content.data->datasize);
			key_aux = malloc(int_aux);
			memcpy(ptr, &key_aux, int_aux);
			ptr += int_aux;
			free(key_aux);
		}

	case CT_RESULT:
		int_aux = htonl(msg->content.result);
		memcpy(ptr, &int_aux, _INT);
		ptr += _INT;
		break;
	}

	return buffer_size;
}

struct message_t *buffer_to_message(char *msg_buf, int msg_size) {

	/* Verificar se msg_buf é NULL */
	if (msg_buf == NULL)
		return NULL;

	//TODO - msg_size < 7?????
	/* msg_size tem tamanho mínimo ? */
	if (msg_size < 7)
		return NULL;

	/* Alocar memória para uma struct message_t */
	struct message_t* msg;
	msg = (struct message_t *) malloc(sizeof(struct message_t));
	if (msg == NULL)
		return NULL;
	/* Recuperar o opcode e c_type */
	short short_aux;
	//short_aux = ntohs(msg->opcode);
	memcpy(&short_aux, msg_buf, _SHORT);
	msg->opcode = ntohs(short_aux);
	msg_buf += _SHORT;

	memcpy(&short_aux, msg_buf, _SHORT);
	msg->c_type = ntohs(short_aux);
	msg_buf += _SHORT;

	/* A mesma coisa que em cima mas de forma compacta, ao estilo C! */
	/*msg->opcode = ntohs(*(short *) msg_buf++);
	 msg->c_type = ntohs(*(short *) ++msg_buf);
	 msg_buf += _SHORT;
	 */
	/* O opcode e c_type são válidos? */
	if (!isValidOPC(msg->opcode) || !isValidCTC(msg->c_type))
		return NULL;
	int int_aux;
	struct data_t* data;
	char *key;
	char** keys;
	struct entry_t* entry;

	/* Consoante o c_type, continuar a recuperação da mensagem original */
	switch (msg->c_type) {
	case CT_RESULT:
		memcpy(&int_aux, msg_buf, _INT);
		msg->content.result = ntohs(int_aux);
		break;
	case CT_VALUE:
		//datasize
		memcpy(&int_aux, msg_buf, _INT);
		msg_buf += _INT;
		//data
		data = data_create2(int_aux, msg_buf);
		if (data == NULL)
			return NULL;
		//TODO DUVIDA ntohost
		msg->content.data = data;
		break;
	case CT_KEY:
		//keysize
		memcpy(&short_aux, msg_buf, _SHORT);
		msg_buf += _SHORT;
		//key
		key = malloc(sizeof(char) * short_aux);
		if (key == NULL)
			return NULL;
		strncpy(key, msg_buf, short_aux);
		//TODO DUVIDA ntohost
		msg->content.key = key;
		break;
	case CT_KEYS:
		//nkeys
		memcpy(&int_aux, msg_buf, _INT);
		msg_buf += _INT;
		keys = (char**) malloc(sizeof(char) * int_aux);
		if (keys == NULL)
			return NULL;
		int i;
		for (i = 0; i < int_aux; i++) {
			//keysize
			memcpy(&short_aux, msg_buf, _SHORT);
			msg_buf += _SHORT;
			//key
			keys[i] = strndup(msg_buf, ntohs(short_aux));
			if (keys[i] == NULL)
				return NULL;
			msg_buf += short_aux;
		}
		msg->content.keys = keys;
		break;
	case CT_ENTRY:
		//keysize
		memcpy(&short_aux, msg_buf, _SHORT);
		msg_buf += _SHORT;
		//key
		key = (char*) malloc(sizeof(char) * short_aux);
		if (key == NULL)
			return NULL;
		strncpy(key, msg_buf, short_aux);
		//TODO DUVIDA - nthost
		msg->content.key = key;
		msg_buf += short_aux;
		//datasize
		memcpy(&int_aux, msg_buf, _INT);
		msg_buf += _INT;
		//data
		data = data_create2(int_aux, msg_buf);
		if (data == NULL)
			return NULL;
		//TODO DUVIDA - ntohost
		msg->content.data = data;
		//recuperar a entry
		entry = entry_create(key, data);
		if (entry == NULL)
			return NULL;
		msg->content.entry = entry;
		break;
	}
	return msg;
}
