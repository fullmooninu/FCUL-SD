/* Sistemas Distribuidos - 2016 - Grupo 34
Elias Miguel Barreira 40821, Pedro Pais 41375
Silvia Ferreira 45511 */

#ifndef _LIST_PRIVATE_H
#define _LIST_PRIVATE_H

#include "entry.h"
#include "list.h"

/* Estrutura que define um noh da lista.
 */
struct node_t {
  struct entry_t *entry;
  struct node_t *next;	/*ponteiro para next*/
};

/* Estrutura que define a lista.
 */
struct list_t{
	struct node_t *head;
	int size;	/* Tamanho da lista */
};

/* Funcao auxiliar para ordernar a lista. 
* Uma especie de bubble sort que so percorre a lista uma vez porque
* assume que a lista estah ordenada ah excepcao dum elemento, 
* e quando o encontra, vai deslocando-o para baixo.
*/
void descending_sort(struct list_t *list);

#endif
