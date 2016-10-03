#include "projeto1_headers/data.h"
#include "projeto1_headers/entry.h"
#include <string.h>
#include <stdlib.h>

/* Função que cria um novo par {chave, valor} (isto é, que inicializa
 * a estrutura e aloca a memória necessária).
 */
struct entry_t *entry_create(char *key, struct data_t *data2){
	if (key == NULL || data2 == NULL) return NULL;
	struct entry_t *e;
	e = malloc(sizeof(struct entry_t)); // falta aqui o ?cast ?
	if(e == NULL) return NULL;
	char *k = strdup(key);
	if(k == NULL) return NULL;
	e->key = k;
	struct data_t *d;
	d = data_create2(data2->datasize, data2->data);
	if(d == NULL) return NULL;
	e->value = d;
	return e;
}

/* Função que destrói um par {chave-valor} e liberta toda a memória.
 */
void entry_destroy(struct entry_t *entry){
	if(entry != NULL){
	free(entry->key);
	data_destroy(entry->value);
	free(entry);
}
}

/* Função que duplica um par {chave, valor}.
 */
struct entry_t *entry_dup(struct entry_t *entry){
	if(entry == NULL) return NULL;
	struct entry_t *copia = entry_create(entry->key, entry->value);
	if(copia == NULL) return NULL;
	return copia;
}
