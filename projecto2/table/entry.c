/* Sistemas Distribuidos - 2016 - Grupo 34 - Proj1:
Elias Miguel Barreira 40821, Pedro Pais 41375
Silvia Ferreira 45511 */


#include "include/data.h"
#include "include/entry.h"
#include <string.h>
#include <stdlib.h>

/* Função que cria um novo par {chave, valor} (isto é, que inicializa
 * a estrutura e aloca a memória necessária).
 */
struct entry_t *entry_create(char *key, struct data_t *data2){
	if (key == NULL || data2 == NULL) return NULL;
	struct entry_t* entry;
	entry = (struct entry_t*) malloc(sizeof(struct entry_t));
	if(entry == NULL) return NULL;
	char* new_key;
	new_key = strdup(key);
	entry->key = new_key;
	struct data_t* new_value;
	new_value = data_create2(data2->datasize,data2->data);
	entry->value = new_value;
	return entry;
}

/* Função que destrói um par {chave-valor} e liberta toda a memória.
 */
void entry_destroy(struct entry_t *entry){
	if(entry != NULL){
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
