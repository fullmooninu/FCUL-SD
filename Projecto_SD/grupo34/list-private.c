/* Sistemas Distribuidos - 2016 - Grupo 34
Elias Miguel Barreira 40821, Pedro Pais 41375
Silvia Ferreira 45511 */

#include "list-private.h"
#include <string.h>

/* Funcao auxiliar para ordernar a lista. 
* Uma especie de bubble sort que so percorre a lista uma vez porque
* assume que a lista estah ordenada ah excepcao dum elemento, 
* e quando o encontra, vai deslocando-o para baixo.
*/
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
}