/* Sistemas Distribuidos - 2016 - Grupo 34 - Proj1:
Elias Miguel Barreira 40821, Pedro Pais 41375
Silvia Ferreira 45511 */


#include "include/list-private.h"
#include "include/entry.h"
#include <stdlib.h>
#include <string.h>

/* Cria uma nova lista. Em caso de erro, retorna NULL.
*/
struct list_t *list_create() {
	struct list_t *list;
	list = (struct list_t*) malloc(sizeof(struct list_t));
	if(list == NULL) return NULL;
	list -> head = NULL;
	list -> size = 0;
	return list;
}

/* Elimina uma lista, libertando *toda* a memoria utilizada pela
* lista.
*/
void list_destroy(struct list_t *list) {
	if(list != NULL){
		struct node_t *current, *next;
		current = list->head;
		while(current != NULL) {
			next = current->next;
			entry_destroy(current->entry);
			free(current);
			current = next;
		}
		free(list);
	}
}

/* Adiciona uma entry na lista. Como a lista deve ser ordenada,
* a nova entry deve ser colocada no local correto.
* Retorna 0 (OK) ou -1 (erro)
*/
int list_add(struct list_t *list, struct entry_t *entry) {

	if(entry == NULL || entry -> value == NULL || entry -> key == NULL)
	return -1;

	struct node_t *newNode;
	newNode = node_create(entry);
	if(newNode == NULL) return -1;

	//se a lista for vazia
	if(list->head == NULL){
		list->head = newNode;
		list->size++;
		return 0;
	}

	//cc
	addBy_descendOrder(newNode, list);
	return 0;
}


/* Elimina da lista um elemento com a chave key.
* Retorna 0 (OK) ou -1 (erro)
*/
int list_remove(struct list_t *list, char* key) {
	if(list == NULL || key == NULL) return -1;
	/* Apontador que vai percorrer os vários nós */
	struct node_t* current = list->head;
	struct node_t* previous = NULL;

	while ( strcmp(current->entry->key, key) != 0) {
		if (current -> next == NULL) {
			return -1;
		}else{
			previous = current;
			current = current -> next;
		}
	}
	if (current == list -> head) {
		list->head = list->head->next;
	}else{
		previous->next = current->next;
	}
	entry_destroy(current->entry);
	free(current->next);
	free(current);
	return 0;
}


/* Obtem um elemento da lista que corresponda a chave key.
* Retorna a referencia do elemento na lista (ou seja, uma alteracao
* implica alterar o elemento na lista).
*/
struct entry_t *list_get(struct list_t *list, char *key) {
	if (list == NULL || key == NULL || list->head == NULL) return NULL;
	struct node_t* current = list->head;

	// correr a lista
	while (current->entry->key != key) {
		//se for o ultimo node
		if (current->next == NULL) {
			return NULL;
		}else{
			current = current->next;
		}
	}
	return current->entry;
}

/* Retorna o tamanho (numero de elementos) da lista
* Retorna -1 em caso de erro.  */
int list_size(struct list_t *list) {
	if (list == NULL || list->size < 0) return -1;
	return list -> size;
}

/* Devolve um array de char * com a copia de todas as keys da
* tabela, e um ultimo elemento a NULL.
*/
char **list_get_keys(struct list_t *list) {
	//TODO mudar isto para dynamic alloc
	struct node_t* current = list -> head;
	//char **list_keys[list -> size + 1];
	char** list_keys;
	list_keys = malloc( (list->size) * sizeof(char**) );
	for (int i = 0; i < list -> size; i++)
	{
		list_keys[i] = current -> entry -> key;
		current = current -> next;
	}
	list_keys[list -> size] = NULL
;	return list_keys;
}

/* Liberta a memoria reservada por list_get_keys.
*/
void list_free_keys(char **keys) {
	for (int i = 0; keys[i] != NULL; i++) {
		free( keys[i]);
	}
	free(keys);
}
