#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "include/table-private.h"
#include "include/list-private.h"


int key_hash(char *key, int l){
	if (key == NULL || l < 1) return -1;

	//FALTA VALIDAR O l que est� associado ao tamanho do Buckets.
	//Ou n�o sera necessario?
	if (key == NULL) return -1;

	unsigned int soma;

	//Pode come�ar em 0.
	soma = 0;

	for(; *key != '0'; key++)
	soma = *key + (soma << 5) - soma;

	return soma % l;
}

struct table_t *table_create(int n) {

	struct table_t *new_table;

	/* n tem valor válido? */
	if (n < 1) return NULL;

	/* Alocar memória para struct table_t */
	new_table =(struct table_t*) malloc(sizeof(struct table_t));

	if(!new_table) return NULL;

	/* Alocar memória para array de listas com n entradas
	que ficará referenciado na struct table_t alocada. */
	if((new_table -> list = (struct list_t **) malloc(n * sizeof(struct list_t *))) == NULL)
	{
		free(new_table-> list);
		return NULL;
	}

	/* Inicializar listas.*/
	unsigned int i;

	for (i = 0; i < n; i++)
		new_table -> list[i] = list_create();

	/* Inicializar atributos da tabela. */
	new_table->size = n;
	new_table->nElem = 0;

	return new_table;
}

void table_destroy(struct table_t *table) {

	if(table != NULL)
	{

		int i;
		struct list_t *list;

		/*   Libertar memória das listas.*/
		for(i=0; i < table->size; i++)
		{
			list = table->list[i];
			while(list != NULL){
				list_destroy(list);
			}
		}

		/*Libertar memória da tabela. */
		free(table-> list);
		free(table);
	}
	else
		return NULL;

}

int table_put(struct table_t *table, char * key, struct data_t *value) {

	/* Verificar valores de entrada */
	if(table == NULL || key == NULL || value == NULL) return -1;

	/* Criar entry com par chave/valor */
	struct entry_t* e = entry_create(key, value);
	if(e == NULL) return -1;

	/* Executar hash para determinar onde inserir a entry na tabela */
	int hashCode = key_hash(key, table->size) % table->size;
	struct list_t* list = table->list[hashCode];
	/* Inserir entry na tabela */
	int retVal = list_add(list,e);
	if(retVal == 0){
		table->size++;
		table->nElem++;
		return 0;
	}
	else
	return -1;
}

int table_update(struct table_t *table, char * key, struct data_t *value) {
	struct data_t* new_data;
	new_data = data_create(sizeof(value));
	int t = sizeof(value);
	memcpy(new_data, value, sizeof(t));

	//TODO como eh que agora descubro qual a lista em que vou fazer list_add
	//pelo keyhash da key?
	return 0;
}

struct data_t *table_get(struct table_t *table, char * key){
	return NULL;
}

int table_del(struct table_t *table, char *key){
	if(table == NULL || key == NULL) return -1;

	int hashCode = key_hash(key, table->size) % table->size;
	struct list_t* list = table->list[hashCode];
	int retVal = list_remove(list, key);
	if(retVal == 0){
		table->nElem--;
		return 0;
	}
	else
	return -1;
}

/* Esta é dada! Ao estilo C! */
int table_size(struct table_t *table) {
	/*ATENCAO QUE NAO PODE SER O SIZE (PEDRO)*/
	return table == NULL ? -1 : table -> nElem;
}

char **table_get_keys(struct table_t *table) {
	if (table == NULL) return NULL;
	struct list_t** current_list = NULL; // como se percorre as listas na tabela? ~ miguel
	char** table_keys_list;
	table_keys_list = malloc( table_size(table) * sizeof(char**));
	for (int i = 0; i < table_size(table); i++)
	{	
		//table_keys_list[i] = list_get_keys(current_list);
		// para cada lista meter a lista toda na char** table_keys
		// e aumentar o index i o correspondente ao n de elementos que se adicionou
	}
	table_keys_list[table->nElem] = NULL;
	return table_keys_list;
}

void table_free_keys(char **keys) {
	list_free_keys(keys);
}
