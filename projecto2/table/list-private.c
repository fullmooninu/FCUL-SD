/* Sistemas Distribuidos - 2016 - Grupo 34 - Proj1:
Elias Miguel Barreira 40821, Pedro Pais 41375
Silvia Ferreira 45511 */

// TODO porque criar este ficheiro?
#include "include/list-private.h"
#include "include/entry.h"
#include "include/list.h"
#include <stdlib.h>
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
  struct entry_t *dupentry;
  dupentry = entry_create(entry->key, entry->value);
  newNode->entry = dupentry;
  newNode->next = NULL;
  return newNode;
}

/* Funcao que recebe um noh, insere-o numa lista e ordena a lista por ordem decrescente
* das chaves alfanumericas
* contidas nas entradas do tipo enty_t.
*/
void addBy_descendOrder(struct node_t *newNode, struct list_t *list){
  struct node_t* currentNode;
  currentNode = list->head;
  //se a key do novo noh for == ah key do current
  if(strcmp(currentNode->entry->key, newNode->entry->key) == 0){
    free(currentNode);
    list->head = newNode;
  } else if(strcmp(currentNode->entry->key, newNode->entry->key) < 0){
    list->head = newNode;
    newNode->next = currentNode;
    list->size++;
  }else{
    while(currentNode->next != NULL &&
      strcmp(currentNode->next->entry->key, newNode->entry->key)>0){
        currentNode = currentNode->next;
      }

      if (currentNode->next == NULL) {
        currentNode->next = newNode;
        list->size++;
      } else {
        if(strcmp(currentNode->next->entry->key, newNode->entry->key) == 0){
          free(currentNode->next);
          currentNode->next = newNode;
        }
        else{
          newNode->next = currentNode->next;
          currentNode->next = newNode;
          list->size++;
        }
      }
    }
  }
