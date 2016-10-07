/* Sistemas Distribuidos - 2016 - Grupo 34 - Proj1:
Elias Miguel Barreira 40821, Pedro Pais 4XXXX
Silvia Ferreira 45511 */

#include "include/list-private.h"
#include <string.h>

/* Funcao que cria um novo elemento da lista node_t, reserva-lhe a memoria
 * necessaria e incializa-o de acordo com o parametro entry
 */
struct node_t *node_create(struct entry_t *entry){
  if(entry == NULL) return NULL;
  struct node_t* newNode;
  newNode = (struct node_t*) malloc(sizeof(struct node_t));
  if(newNode == NULL) return NULL;
  //inicializacao do newNode
  newNode->entry = entry;
  newNode->next = NULL;
  return newNode;
}

/* Funcao que recebe um noh, insere-o numa lista e ordena a lista por ordem decrescente
* das chaves alfanuméricas
* contidas nas entradas do tipo enty_t.
*/
void addBy_descendOrder(struct node_t *newNode, struct struct list_t *list){
  struct node_t* currentNode;
  currentNode = list->head;
  //se a key do novo noh for > que a key do current, colocamos o novo noh logo à cabeça
  if(strcmp(currentNode->entry->key, newNode->entry->key) < 0){
    list->head = newNode;
    newNode->next = currentNode;
    list->size++;
  }else{
    while(currentNode->next != NULL &&
      strcmp(currentNode->entry->key, newNode->entry->key)>0){
        currentNode = currentNode->next;
      }
      newNode->next = currentNode->next;
      currentNode->next = newNode;
      list->size++;
    }
  }
