#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "table-private.h"
#include "include/list-private.h"


int key_hash(char *key, int l){
	if (key == NULL || l < 1) return -1;

	//FALTA VALIDAR O l que est· associado ao tamanho do Buckets.
	//Ou n„o sera necessario?
	if (key == NULL) return -1;

	unsigned int soma;

	//Pode comeÁar em 0.
	soma = 0;

	for(; *key != '0'; key++)
		soma = *key + (soma << 5) - soma;

  return soma % l;
}

struct table_t *table_create(int n) {

	struct table_t *new_table;

  /* n tem valor v√°lido? */
	if (n < 1) return NULL;

  /* Alocar mem√≥ria para struct table_t */
	new_table = malloc(sizeof(table_t));


  /* Alocar mem√≥ria para array de listas com n entradas
     que ficar√° referenciado na struct table_t alocada. */
	int buckets[n + 1];
	new_table -> buckets = buckets;


    /* Inicializar listas.*/
	new_table -> list = malloc(sizeof(list_t *) * n);

	int i;
	for (i = 0; i < n; i++)
		new_table -> list[i] = NULL;

    /* Inicializar atributos da tabela. */
	new_table->size = n;
	new_table->nListas = 0;

  return new_table;
}

void table_destroy(struct table_t *table) {

  /* table √© NULL? */
	if(table == NULL) return NULL;

	int i;
	struct list_t *list;

  /*   Libertar mem√≥ria das listas.*/
	for(i=0; i < table->size;i++)
	{
		list = table->list[i];
		while(list != NULL){
			list_destroy(list);
		}
	}

     /*Libertar mem√≥ria da tabela. */
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

/* Esta √© dada! Ao estilo C! */
int table_size(struct table_t *table) {
	/*ATEN«√O QUE N¬O PODE SER O SIZE (PEDRO)*/
	return table == NULL ? -1 : table->nListas;
}

char **table_get_keys(struct table_t *table) {

}

void table_free_keys(char **keys) {

}

