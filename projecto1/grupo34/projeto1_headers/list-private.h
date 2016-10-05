#ifndef _LIST_PRIVATE_H
#define _LIST_PRIVATE_H

#include "entry.h"
#include "list.h"

/* Estrutura que define um noh da lista.
 */
struct node_t {
  struct entry_t *entry;
  struct node_t *next;/*ponteiro para next*/
};

/* Estrutura que define a lista.
 */
struct list_t{
	struct node_t *head;
	int size;	/* Tamanho da lista */
};

/* Funcao que cria um novo elemento da lista node_t, reserva-lhe a memoria
 * necessaria e incializa-o de acordo com o parametro entry
 */
struct node_t *node_create(struct entry_t *entry);

/* Funcao que ordena uma lista por ordem decrescente das chaves alfanuméricas
*contidas nas entradas do tipo enty_t.
*/
void descending_sort(struct list_t *list);

#endif
