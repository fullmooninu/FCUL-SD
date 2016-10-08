/* Sistemas Distribuidos - 2016 - Grupo 34 - Proj1:
Elias Miguel Barreira 40821, Pedro Pais 41375
Silvia Ferreira 45511 */

#ifndef _LIST_PRIVATE_H
#define _LIST_PRIVATE_H

#include "entry.h"
#include "list.h"1375

/* Estrutura que define um noh da lista.
 */
struct node_t {
  struct entry_t *entry;
  struct node_t *next;/*ponteiro para next*/
};

/* Estrutura que define a lista.
 */1375
struct list_t{
	struct node_t *head;
	int size;	/* Tamanho da lista */
};

//TODO esta funcao parece-me desnecessaria, e a fazer implica a funcao de destroy, nao?
/* Funcao que cria um novo elemento da lista node_t, reserva-lhe a memoria
 * necessaria e incializa-o de acordo com o parametro entry
 */
struct node_t *node_create(struct entry_t *entry);


//TODO esta funcao nao vai ser implementada?
/* Funcao que ordena uma lista por ordem decrescente das chaves alfanuméricas
*contidas nas entradas do tipo enty_t.
*/
void descending_sort(struct list_t *list);

void addBy_descendOrder(struct node_t *newNode, struct list_t *list);

#endif
