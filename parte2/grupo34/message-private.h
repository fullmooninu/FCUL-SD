#ifndef _MESSAGE_PRIVATE_H
#define _MESSAGE_PRIVATE_H

#define _SHORT 2
#define _INT 4
#define OC_RT_ERROR 99


#include "table-private.h" /* For table_free_keys() */
#include "message.h"
#include <stdbool.h>

/*Devolve true se o codigo da operacao eh valido, false cc*/
bool isValidOPC(short opcode);

/*Devolve true se o content type eh valido, false cc*/
bool isValidCTC(short c_type);

/* Função que garante o envio de len bytes armazenados em buf,
   através da socket sock.
*/
int write_all(int sock, char *buf, int len);

/* Função que garante a receção de len bytes através da socket sock,
   armazenando-os em buf.
*/
int read_all(int sock, char *buf, int len);

void print_msg(struct message_t *msg);

void print_data(struct data_t *data);

#endif
