#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "table-private.h"

int key_hash(char *key, int l){
	if (key == NULL | l > size) return -1;

	unsigned int soma;

	//Pode come鏰r em 0.
	soma = 0;

	for(; *key != '0'; key++)
		soma = *key + (soma << 5) - soma;

  return soma % l;
}

struct table_t *table_create(int n) {

	struct table_t *new_table;

  /* n tem valor v谩lido? */
	if (n < 1) return NULL;

  /* Alocar mem贸ria para struct table_t */
	new_table = malloc(sizeof(table_t));


  /* Alocar mem贸ria para array de listas com n entradas 
     que ficar谩 referenciado na struct table_t alocada. */
	new_table -> buckets = int buckets[n+1];


    /* Inicializar listas.*/
	new_table -> list_t = malloc(sizeof(struct list_t *) * size);

	int i;
	for (i = 0; i < size; i++)
		new_table -> list[i] = NULL;

    /* Inicializar atributos da tabela. */
	new_table->size = n;
	new_table->nListas = 0;

  return new_table;
}

void table_destroy(struct table_t *table) {

  /* table 茅 NULL? */
	if(table == NULL) return -1;
	int i;

  /*   Libertar mem贸ria das listas.*/
	for(i=0; i < table->size;i++)
	{
		list = table->list[i];
		while(list != NULL){
			list_destroy(list);
		}
	}

     /*Libertar mem贸ria da tabela. */
	free(table->buckets);
	free(table->list);
	free(table);

  
}

int table_put(struct table_t *table, char * key, struct data_t *value) {

  /* Verificar valores de entrada */

  /* Criar entry com par chave/valor */

  /* Executar hash para determinar onde inserir a entry na tabela */

  /* Inserir entry na tabela */

}

int table_update(struct table_t *table, char * key, struct data_t *value) {

}

struct data_t *table_get(struct table_t *table, char * key){

}

int table_del(struct table_t *table, char *key){

}

/* Esta 茅 dada! Ao estilo C! */
int table_size(struct table_t *table) {
	/*ATEN敲O QUE N翺 PODE SER O SIZE (PEDRO)*/
	return table == NULL ? -1 : table->nListas;
}

char **table_get_keys(struct table_t *table) {

}

void table_free_keys(char **keys) {

}

