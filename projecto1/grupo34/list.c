/* Sistemas Distribuidos - 2016 - Grupo 34 - Proj1:
Elias Miguel Barreira 40821, Pedro Pais 41375
Silvia Ferreira 45511 */


#include "include/list-private.h"
#include "include/entry.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
	if(list != NULL) free(list);
}

/* Adiciona uma entry na lista. Como a lista deve ser ordenada,
* a nova entry deve ser colocada no local correto.
* Retorna 0 (OK) ou -1 (erro)
*/
int list_add(struct list_t *list, struct entry_t *entry) {
	if(list == NULL || entry == NULL || entry -> value == NULL || entry -> key == NULL) {
		return -1;
	}
	if (list->size==0) { 
		struct node_t* new_head = (struct node_t*) malloc(sizeof(struct node_t));
		list->head = new_head;
		list->head->entry=entry_create(entry->key,entry->value);
		list->head->next=NULL;
		list->size = 1;
		return 0;
	}else{
	//create a link
	struct node_t* new_head = (struct node_t*) malloc(sizeof(struct node_t));
	new_head->next = list->head;
	new_head->entry = entry_create(entry->key,entry->value);

	list->head=new_head;
	list->size += 1;	
	//descending_sort(list);
	return 0;
	}
}


void descending_sort(struct list_t *list) {
	struct node_t* current_node;
	struct node_t* next_node;
	current_node = list->head;
	next_node = current_node->next;

	struct entry_t* tempEntry;

	while(next_node!=NULL) {
		if ( strcmp(current_node->entry->key , next_node->entry->key) < 0 ) {
 			tempEntry = current_node->entry;
 			current_node->entry = next_node->entry;
 			next_node->entry = tempEntry;
		}
		current_node = current_node->next;
		next_node = current_node->next;
	}

	// struct node_t* current_node;
	// struct node_t* next_node;
	// struct entry_t* tempEntry;
	// int k = list->size;
	

	// for (int i = 0; i < list->size - 1; i++, k--) {
	// 	current_node = list->head;
	// 	next_node = list->head->next;
	// 	for (int j = 1; j < k; j++) {
	// 		if ( strcmp(current_node->entry->key , next_node->entry->key) > 0) {
	// 			tempEntry = current_node->entry;
	// 			current_node->entry = next_node->entry;
	// 			next_node->entry = tempEntry;
	// 		}
	// 	}
	// }
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
	// entry_destroy(current->entry);
	// free(current->next->);
	free(current);
	list->size -= 1;
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
	while (strcmp(current->entry->key, key) != 0) {
		//se for o ultimo node
		if (current->next == NULL) {
			return NULL;
		}else{
			current = current->next;
		}
	}
	struct entry_t* ret_entry;
	ret_entry = entry_create(current->entry->key,current->entry->value);
	return ret_entry;
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
	if(list == NULL) return NULL;
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
	if (keys == NULL) return;
	for (int i = 0; keys[i] != NULL; i++) {
		free( keys[i]);
	}
	//free(keys);
}
