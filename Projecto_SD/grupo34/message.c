#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <errno.h>
#include "message-private.h"
#include "message.h"
#include <stdlib.h>
#include <unistd.h>

void free_message(struct message_t *msg) {
	if (msg == NULL) return;

	if (msg->c_type == CT_ENTRY) {
		entry_destroy(msg->content.entry);
	}
	if (msg->c_type == CT_KEY) {
		free(msg->content.key);
	}
	if (msg->c_type == CT_KEYS) {
		for (int i = 0; msg->content.keys[i] != NULL; i++) {
			free(msg->content.keys[i]);
		}
		free(msg->content.keys);
	}
	if (msg->c_type == CT_VALUE) {
		data_destroy(msg->content.data);
	}

	free(msg);
}

int message_to_buffer(struct message_t *msg, char **msg_buf) {

	/* Verificar se msg é NULL */
	if (msg == NULL || msg_buf == NULL)
		return -1;

	/* Consoante o msg->c_type, determinar o tamanho do vetor de bytes
	 que tem de ser alocado antes de serializar msg
	 */
	int buffer_size = 0; //Tamanho em bytes

	//Variavel auxiliar para o tamanho de cada chave (CT_KEYS)
	int keysSize = 0;

	//REVER TAMANHOS
	switch (msg->c_type) {  //
	case CT_ENTRY:
		buffer_size = 6 + strlen(msg->content.entry->key) + 4;
		if (msg->content.entry->value->datasize > 0)
			buffer_size += msg->content.entry->value->datasize;
		break;
	case CT_KEY:
		buffer_size = (6 + strlen(msg->content.key));
		break;
	case CT_KEYS:
		for (int i = 0; msg->content.keys[i] != NULL; i++) {
			keysSize += 2 + strlen(msg->content.keys[i]);
		}
		buffer_size = 8 + keysSize;
		break;
	case CT_VALUE:
		buffer_size = 8;
		if ((msg->content.data->datasize) != 0)
			buffer_size += msg->content.data->datasize;
		break;
	case CT_RESULT:
		buffer_size = 8;
		break;
	}

	char* buffer;
	buffer = (char*) malloc(buffer_size);
	if (buffer == NULL) {
		*msg_buf = NULL;
		return -1;
	}
	char* i_buffer_loc = buffer;

	short short_value = htons(msg->opcode);
	memcpy(buffer, &short_value, _SHORT); // short = 2
	buffer += _SHORT;

	short_value = htons(msg->c_type);
	memcpy(buffer, &short_value, _SHORT);
	buffer += _SHORT;

	/* Consoante o conteúdo da mensagem, continuar a serialização da mesma */

	int int_aux = 0;
	short short_aux = 0;
	int tamanhoDaChave = 0;
	int tamanhoDaData;
	char* key_aux;

	//Variavel auxiliar para saber o numero de keys
	int nKeys = 0;

	switch (msg->c_type) {
	case CT_ENTRY:
		tamanhoDaChave = strlen(msg->content.entry->key);
		short_aux = htons(tamanhoDaChave);
		memcpy(buffer, &short_aux, _SHORT);
		buffer += _SHORT;
		//colocar a chave
		memcpy(buffer, msg->content.entry->key, tamanhoDaChave);
		buffer += tamanhoDaChave;
		//colocar o data_sizeDS
		tamanhoDaData = msg->content.entry->value->datasize;
		int_aux = htonl(tamanhoDaData);
		memcpy(buffer, &int_aux, _INT);
		buffer += _INT;
		if (tamanhoDaData > 0) {
			//colocar o data->data
			memcpy(buffer, msg->content.entry->value->data, tamanhoDaData);
			buffer += tamanhoDaData;
		}
		break;
	case CT_KEY:
		short_aux = htons(strlen(msg->content.key));
		memcpy(buffer, &short_aux, _SHORT);
		buffer += _SHORT;
		//colocar a chave
		memcpy(buffer, msg->content.key, strlen(msg->content.key));
		buffer += strlen(msg->content.key); //j� sabemos que o strlen nao contabiliza o '/0'
		break;

	case CT_KEYS:
		key_aux = msg->content.keys[0];
		for (int i = 0; msg->content.keys[i] != NULL; i++) {
			nKeys++;
			key_aux += strlen(msg->content.keys[i]);
		}
		int_aux = htonl(nKeys);
		memcpy(buffer, &int_aux, _INT);
		buffer += _INT;
		//colocar as varias chaves
		for (int i = 0; msg->content.keys[i] != NULL; i++) {
			tamanhoDaChave = htons(strlen(msg->content.keys[i]));
			memcpy(buffer, &tamanhoDaChave, _SHORT);
			buffer += _SHORT;
			memcpy(buffer, msg->content.keys[i], tamanhoDaChave);
			buffer += tamanhoDaChave;
		}
		break;

	case CT_VALUE:
		//colocar o data_sizeDS
		int_aux = htonl(msg->content.data->datasize);
		memcpy(buffer, &int_aux, _INT);
		buffer += _INT;
		if ((msg->content.data->datasize) > 0) {
			//colocar o data->data
			int_aux = msg->content.data->datasize;
			memcpy(buffer, msg->content.data->data, int_aux);
			buffer += int_aux;
		}

	case CT_RESULT:
		int_aux = htonl(msg->content.result);
		memcpy(buffer, &int_aux, _INT);
		buffer += _INT;
		break;
	}

	*msg_buf = i_buffer_loc;
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
	if (!isValidOPC(msg->opcode) && !isValidCTC(msg->c_type))
		return NULL;
	int int_aux;
	struct data_t* data2;
	char *key;
	char *keys;
	int tamanhoDaData = 0;
	int nKeys = 0;
	short keysize = 0;

	/* Consoante o c_type, continuar a recuperação da mensagem original */
	switch (msg->c_type) {
	case CT_RESULT:
		memcpy(&int_aux, msg_buf, _INT);
		msg->content.result = ntohl(int_aux);
		msg_buf += _INT;
		break;
	case CT_VALUE:
		//datasize
		memcpy(&int_aux, msg_buf, _INT);
		tamanhoDaData = ntohl(int_aux);
		msg_buf += _INT;
		//data
		msg->content.data = data_create2(tamanhoDaData, msg_buf);
		break;
	case CT_KEY:
		//keysize
		memcpy(&short_aux, msg_buf, _SHORT);
		short_aux = ntohs(short_aux);
		msg_buf += _SHORT;
		//key
		key = strndup(msg_buf, short_aux);
    if (key == NULL) return NULL;
		msg->content.key = key;
		break;
	case CT_KEYS:
		//nkeys

		memcpy(&int_aux, msg_buf, _INT);
		nKeys = ntohl(int_aux);
		msg_buf += _INT;

		msg->content.keys = (char**) malloc((nKeys + 1) * sizeof(char*));
		for (int i = 0; i < nKeys; i++) {

			//keysize
			memcpy(&short_aux, msg_buf, _SHORT);
			keysize = ntohs(short_aux);
			msg_buf += _SHORT;
			//key
			msg->content.keys[i] = malloc(keysize + 1);
			memcpy(msg->content.keys[i], msg_buf, keysize);
			msg->content.keys[i][keysize] = '\0';
			msg_buf += keysize;

		}
		msg->content.keys[nKeys] = NULL;
		break;

	case CT_ENTRY:
		//keysize /
		memcpy(&short_aux, msg_buf, _SHORT);
		keysize = ntohs(short_aux);
		msg_buf += _SHORT;
		//key
		key = strdup(msg_buf + keysize);
		msg_buf += keysize;
		//datasize
		memcpy(&int_aux, msg_buf, _INT);
		tamanhoDaData = ntohl(int_aux);
		msg_buf += _INT;
		//data
		if (tamanhoDaData > 0) {
			void *data = malloc(tamanhoDaData);
			memcpy(data, msg_buf, tamanhoDaData);
			msg_buf += tamanhoDaData;
			data2 = data_create2(tamanhoDaData, data);
			msg->content.entry = entry_create(key, data2);
			free(data);
		}
		break;
	}
	return msg;
}

int read_all(int sock, char *buf, int len) {

	int bufsize = len;
	while (len > 0) {
		int res = read(sock, buf, len);
		if (res < 0) {
			int errsv = errno;
			if (errsv == EINTR)
				continue;
			perror("Read failed!");
			return res;
		}
		buf += res;
		len -= res;
	}
	return bufsize;
}

int write_all(int sock, char *buf, int len) {
	int bufsize = len;
	while (len > 0) {
		int res = write(sock, buf, len);
		if (res < 0) {
			int errsv = errno;
			if (errsv == EINTR)
				continue;
			perror("Write failed!");
			return res;
		}
		buf += res;
		len -= res;
	}
	return bufsize;
}
