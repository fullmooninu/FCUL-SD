/* Sistemas Distribuidos - 2016 - Grupo 34 - Proj1:
Elias Miguel Barreira 40821, Pedro Pais 4XXXX
Silvia Ferreira 45511 */


#include "include/list-private.h"
#include <stdlib.h>


/* Cria uma nova lista. Em caso de erro, retorna NULL.
*/
struct list_t *list_create() {
	struct list_t *list;
	list = (struct list_t*) malloc(sizeof (struct list_t));
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

/* novo metodo para fazer sort a lista*/
void sort() {
	// TODO
	//criei em alternativa o addBy_descendOrder
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
	//FALTA COMPLETAR
	if(list == NULL || key == NULL) return -1;
	/* Apontador que vai percorrer os varios nos */
	struct node_t *current = head;
	while (strcmp(current -> entry -> key,*key) != 0)
	current = head.next;
	/* Se chegar ao fim e n�o estiver lah a chave, da erro. */
	if(strcmp(current -> entry -> key, *key) != 0) return -1;

	entry_destroy(current); //TEM-SE QUE FAZER O INCLUDE DO ENTRY.H?
	//FALTA APAGAR N� E JUNTAR OS NEXT

	return 0;
}


/* Obtem um elemento da lista que corresponda à chave key.
* Retorna a referência do elemento na lista (ou seja, uma alteração
* implica alterar o elemento na lista).
*/
struct entry_t *list_get(struct list_t *list, char *key) {
	if (list == NULL || key == NULL) return NULL;
	struct node_t* current = list -> head;

	if (list -> head == NULL) return NULL;

	// correr a lista
	while (current -> entry -> key != key) {
		//se for o ultimo node
		if (current -> next == NULL) {
			return NULL;
		}else{
			current = current -> next;
		}
	}
	return current -> entry;
}

/* Retorna o tamanho (numero de elementos) da lista
* Retorna -1 em caso de erro.  */
int list_size(struct list_t *list) {
	if (list == NULL || list->size < 0) return -1;
	return list -> size;
}

/* Devolve um array de char * com a cópia de todas as keys da
* tabela, e um último elemento a NULL.
*/
char **list_get_keys(struct list_t *list) {
	//TODO mudar isto para dynamic alloc
	struct node_t* current = list -> head;
	char **list_keys[list -> size + 1];
	for (int i = 0; i < list -> size; i++) 
	{
		list_keys[i] = current -> entry -> key;
		current = current -> next;
	}
	list_keys[list -> size] = NULL;
	return list_keys;
}

/* Liberta a memoria reservada por list_get_keys.
*/
void list_free_keys(char **keys) {
	free(keys);
}
