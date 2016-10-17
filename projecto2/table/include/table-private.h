#ifndef _TABLE_PRIVATE_H
#define _TABLE_PRIVATE_H

#include "table.h"

struct table_t{
	int nElem; /* numero de elementos que tem a tabela */
	struct list_t **list; /*as listas das tabelas >> de forma dinamica.*/
	int size; /* Dimensão da tabela */
} table_t;

int key_hash(char *key, int l);

#endif
