/* Sistemas Distribuidos - 2016 - Grupo 34
Elias Miguel Barreira 40821, Pedro Pais 41375
Silvia Ferreira 45511 */


#include "message-private.h"
#include "message.h"
#include <stdio.h>
#include <stdlib.h>

bool isValidOPC(short opcode){
	return (opcode == OC_SIZE || opcode == OC_DEL || opcode == OC_UPDATE
		|| opcode == OC_GET || opcode == OC_PUT);
}
bool isValidCTC(short c_type){
	return (c_type == CT_RESULT || c_type == CT_VALUE || c_type == CT_KEY
		|| c_type == CT_KEYS || c_type == CT_ENTRY);
}


void print_msg(struct message_t *msg) {
	printf("----- MESSAGE -----\n");
	printf("opcode: %d, c_type: %d\n", msg->opcode, msg->c_type);
	switch(msg->c_type) {

		case CT_ENTRY:{
			// char *c;
			printf("key: %s\n", msg->content.entry->key);
			print_data(msg->content.entry->value);
			// printf("datasize: %d\n", msg->content.entry->value->datasize);
			// printf("data: ");
			// c = (char *) msg->content.entry->value->data;
			// for (i = 0; i < msg->content.entry->value->datasize; i++) {
			// 	printf("%c", *c);
			// 	c += 1;
			// }
			// printf("\n");
      // printf("data: %s\n", msg->content.entry->value->data);
		}break;
		case CT_KEY:{
			printf("key: %s\n", msg->content.key);
		}break;
		case CT_KEYS:{
			for(int i = 0; msg->content.keys[i] != NULL; i++) {
				printf("key[%d]: %s\n", i, msg->content.keys[i]);
			}
		}break;
		case CT_VALUE:{
			print_data(msg->content.data);
		}break;
		case CT_RESULT:{
			printf("result: %d\n", msg->content.result);
		};
	}
	printf("-------------------\n");
}

void print_data(struct data_t *data) {
	int i;
	char *c;
	c = (char *) data->data;
	printf("datasize: %d\n", data->datasize);
	printf("data: ");
	for (i = 0; i < data->datasize; i++) {
		printf("%c", *c);
		c += 1;
	}
	printf("\n");
}
