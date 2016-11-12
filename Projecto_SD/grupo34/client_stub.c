/* Sistemas Distribuidos - 2016 - Grupo 34
Elias Miguel Barreira 40821, Pedro Pais 41375
Silvia Ferreira 45511 */

#include <stdio.h>
#include "inet.h"
#include "client_stub.h"
#include "client_stub-private.h"
#include "list.h"
#include "list-private.h"
#include "table.h"
#include <errno.h>

//
struct rtable_t *rtable_bind(const char *address_port){
	struct rtable_t *rtable = (struct rtable_t*) malloc(sizeof(struct rtable_t));

	rtable->server = (struct server_t*)malloc(sizeof(struct server_t));

	rtable->msg = (struct message_t*)malloc(sizeof(struct message_t));
	rtable->server = network_connect(address_port);

	return rtable;
}

int rtable_unbind(struct rtable_t *rtable){
	if (network_close(rtable->server) == 0){
		return 0;
	}
	else
		return(-1);
}

int rtable_put(struct rtable_t *rtable, char *key, struct data_t *value) {

	struct data_t *valueN = (struct data_t*)malloc(sizeof(struct data_t));
	valueN = data_create2(value->datasize,value);
	struct entry_t *entry =(struct entry_t*)malloc(sizeof(struct entry_t));
	entry = entry_create(key, valueN);
	//Para a mensagem
	struct message_t *mensagem = (struct message_t*)malloc(sizeof(struct message_t));
	mensagem -> opcode = OC_PUT;
	mensagem -> c_type = CT_ENTRY;
	mensagem -> content.entry = entry;

	if( network_send_receive(rtable->server, mensagem)->opcode == OC_PUT+1){
		free(valueN);
		free(entry);
		return 0;
	}
	else{
		return (-1);
	}

}

//TODO FALTA FAZER A FUNÇAO UPDATE
int rable_update(struct rtable_t *rtable, char *key, struct data_t *value){
//TODO
}


struct data_t *rtable_get(struct rtable_t *table, char *key) {
	struct message_t *mensagem = (struct message_t*)malloc(sizeof(struct message_t));
	mensagem->opcode = OC_GET;
	mensagem->content.key=(char *)malloc(sizeof(char));
	mensagem->c_type = CT_KEY;
	mensagem->content.key=key;

	if (network_send_receive(table->server,mensagem)->opcode == OC_GET+1){
		return (network_send_receive(table->server,mensagem)->content.data);

	}
	else{
		return NULL;
	}
}


int rtable_del(struct rtable_t *table, char *key){
	struct message_t *mensagem = (struct message_t*)malloc(sizeof(struct message_t));
	mensagem->opcode = OC_DEL;
	mensagem->c_type = CT_KEY;
	mensagem->content.key = (char *)malloc(sizeof(char));
	mensagem->content.key = key;

	if(network_send_receive(table->server, mensagem)->opcode == OC_DEL+1){
		return (0);
	}
	else
		return (-1);
}

int rtable_size(struct rtable_t *rtable){
	struct message_t *mensagem = (struct message_t*)malloc(sizeof(struct message_t));

	mensagem->opcode = OC_SIZE;
	mensagem->c_type = CT_RESULT;
	mensagem->content.result = 0;

	if(network_send_receive(rtable->server, mensagem)->opcode == OC_SIZE+1){
		return (network_send_receive(rtable->server,mensagem)->content.result);
	}
	else{
		return (-1);
	}
}
char **rtable_get_keys(struct rtable_t *rtable){
	struct message_t *mensagem = (struct message_t*)malloc(sizeof(struct message_t));

	mensagem->opcode = OC_GET;
	mensagem->c_type = CT_KEYS;

	if(network_send_receive(rtable->server, mensagem)->opcode==OC_GET+1){
		return (network_send_receive(rtable->server, mensagem)->content.keys);

	}
	else
		return NULL;
}
