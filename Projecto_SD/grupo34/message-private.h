#ifndef _MESSAGE_PRIVATE_H
#define _MESSAGE_PRIVATE_H

#define _SHORT 2
#define _INT 4

#include "table-private.h" /* For table_free_keys() */
#include "message.h"
#include <stdbool.h>

/*Devolve true se o codigo da operacao eh valido, false cc*/
bool isValidOPC(short opcode);

/*Devolve true se o content type eh valido, false cc*/
bool isValidCTC(short c_type);

#endif
