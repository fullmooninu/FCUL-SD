/* Sistemas Distribuidos - 2016 - Grupo 34
Elias Miguel Barreira 40821, Pedro Pais 41375
Silvia Ferreira 45511 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "table-private.h"


int key_hash(char *key, int l){
	if (key == NULL || l < 1) return -1;

	int i, key_len = strlen(key);
	int soma = 0;
	if(key_len <= 6){
		for(i = 0; i < key_len-1; i++){
			soma += (int) key[i];
		}
	}
	else{
		for(i = 0; i < 3; i++){
			soma += (int) key[i];
		}
		for(i = key_len-1; i < key_len-4; i--){
			soma += (int)key[i];
		}
	}
	return soma % l;

}

/* Função para criar/inicializar uma nova tabela hash, com n
* linhas(n = módulo da função hash)
*/
struct table_t *table_create(int n) {
	struct table_t *new_table;
	// n tem valor valido?
	if (n < 1) return NULL;
	// Alocar memoria para struct table_t
	new_table =(struct table_t*) malloc(sizeof(struct table_t));
	if(!new_table) return NULL;

	/* Alocar memoria para array de listas com n entradas
	que ficara referenciado na struct table_t alocada. */
	if((new_table->list = (struct list_t **) malloc(n * sizeof(struct list_t*))) == NULL) 
	{	free(new_table-> list);
		free(new_table);
		return NULL;
	}

		/* Inicializar listas.*/
	
	int i;

	for (i = 0; i < n; i++)
		new_table -> list[i] = NULL;
	
		/* Inicializar atributos da tabela. */
	new_table->size = n;
	new_table->nElem = 0;

	return new_table;
}

/* Libertar toda a memória ocupada por uma tabela.
*/
void table_destroy(struct table_t *table) {
	if(table != NULL) {
		/*   Libertar memória das listas.*/
		for(int i=0; i < table->size; i++) {
			list_destroy(table->list[i]);
		}
		/*Libertar memória da tabela. */
		free(table-> list);
		free(table);
	}
}

/* Função para adicionar um par chave-valor na tabela.
* Os dados de entrada desta função deverão ser copiados.
* Devolve 0 (ok) ou -1 (out of memory, outros erros)
*/
int table_put(struct table_t *table, char * key, struct data_t *value) {
	/* Verificar valores de entrada */
	if(table == NULL || key == NULL || value == NULL) return -1;
	
	/* Executar hash para determinar onde inserir a entry na tabela */
	int hash = key_hash(key, table->size);

	// Verificar se jah existe
	if (list_get(table->list[hash],key) != NULL) return 0;
	
	/* Criar entry com par chave/valor */
	struct entry_t* e;
	e = entry_create(key, value);
	if(e == NULL) return -1; 
	
	// se nao existir lista no local, criar nova lista
	if(table->list[hash] == NULL){
		table->list[hash] = list_create();
		if(table->list[hash] == NULL) return -1;
	}
	/* Inserir entry na tabela */
	int retVal = list_add(table->list[hash],e);
	entry_destroy(e);
	if(retVal == 0){
		table->nElem++;
		return 0;
	}
	else
		return -1;
}

/* Função para substituir na tabela, o valor associado à chave key.
* Os dados de entrada desta função deverão ser copiados.
* Devolve 0 (OK) ou -1 (out of memory, outros erros)
*/
int table_update(struct table_t *table, char * key, struct data_t *value) {
	// Verificar valores de entrada
	if(table == NULL || key == NULL || value == NULL) return -1;

	// Criar entry com par chave/valor
	struct entry_t* e = entry_create(key, value);
	if(e == NULL) return -1;

    // Executar hash para determinar onde inserir a entry na tabela */
	int hashCode = key_hash(key, table->size);
	struct list_t* list = table->list[hashCode];
		/* Inserir entry na tabela */
	int retVal = list_add(list,e);
	entry_destroy(e);
	if (retVal == 0) {
		return 0;
	}else{
		return 1;
	}
}

/* Função para obter da tabela o valor associado à chave key.
* A função deve devolver uma cópia dos dados que terão de
* ser libertados no contexto da função que chamou table_get.
* Devolve NULL em caso de erro.
*/
struct data_t *table_get(struct table_t *table, char * key){
	if(table == NULL || key == NULL) return NULL;
	int hash = key_hash(key, table->size);
	
	struct entry_t* entry;
	entry = list_get(table->list[hash],key);
	if (entry == NULL) return NULL;

	struct data_t* ret_data;
	ret_data = data_dup(list_get(table->list[hash],key)->value);
	if(ret_data == NULL) return NULL;

	
	return ret_data;
}

/* Função para remover um par chave valor da tabela, especificado
* pela chave key, libertando a memória associada a esse par.
* Devolve: 0 (OK), -1 (nenhum tuplo encontrado; outros erros)
*/
int table_del(struct table_t *table, char *key){
	if(table == NULL || key == NULL) return -1;

	int hashCode = key_hash(key, table->size);
	struct list_t* list = table->list[hashCode];
	int retVal = list_remove(list, key);
	if(retVal == 0){
		table->nElem--;
		return 0;
	}
	else
		return -1;
}

/* Devolve o número de elementos na tabela.
*/
int table_size(struct table_t *table) {
	return table == NULL ? -1 : table->nElem;
}

/* Devolve um array de char * com a cópia de todas as keys da
* tabela, e um último elemento a NULL.
*/
char **table_get_keys(struct table_t *table) {
	if (table == NULL) return NULL;
	//  Inicializar returned value
	char** table_keys = NULL;
	table_keys = malloc( (table->nElem + 1) * sizeof(char*));
	if (table_keys == NULL) return NULL;
	
	int pos = 0;
	for (int i = 0; i < table->size; i++) {
		if(table->list[i] == NULL) continue;
		char** keys;
		keys = list_get_keys(table->list[i]);
		for(int f = 0; keys[f]!=NULL; f++) {
				table_keys[pos] = malloc(strlen(keys[f])+1);
				strcpy(table_keys[pos],keys[f]);
				pos++;
		} 
		list_free_keys(keys);
	}
	table_keys[table->nElem] = NULL;
	return table_keys;
}

/* Liberta a memória alocada por table_get_keys().
*/
void table_free_keys(char **keys) {
		list_free_keys(keys);
}
