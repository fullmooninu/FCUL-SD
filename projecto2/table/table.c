#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "table-private.h"


int key_hash(char *key, int l){
<<<<<<< HEAD
	if (key == NULL || l < 1) return -1;

	//FALTA VALIDAR O l que est� associado ao tamanho do Buckets.
	//Ou n�o sera necessario?
=======
	if (key == NULL) return -1;
>>>>>>> branch 'master' of https://github.com/fullmooninu/FCUL-SD.git

	/* FALTA VALIDAR l - ESTÁ ASSOCIADO AO SIZE DA DO BUCKET */
	
	
	unsigned int soma;

	//Pode começar em 0.
	soma = 0;

	for(; *key != '0'; key++)
		soma = *key + (soma << 5) - soma;

  return soma % l;
}

struct table_t *table_create(int n) {

	struct table_t *new_table;

  /* n tem valor vÃ¡lido? */
	if (n < 1) return NULL;

  /* Alocar memÃ³ria para struct table_t */
	new_table = malloc(sizeof(table_t));


<<<<<<< HEAD
  /* Alocar memória para array de listas com n entradas
     que ficará referenciado na struct table_t alocada. */
	int buckets[n + 1];
	new_table -> buckets = buckets;
=======
  /* Alocar memÃ³ria para array de listas com n entradas 
     que ficarÃ¡ referenciado na struct table_t alocada. */
	new_table -> buckets = int buckets[n+1];
>>>>>>> branch 'master' of https://github.com/fullmooninu/FCUL-SD.git


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

  /* table Ã© NULL? */
	if(table == NULL) return -1;
	int i;

  /*   Libertar memÃ³ria das listas.*/
	for(i=0; i < table->size;i++)
	{
		list = table->list[i];
		while(list != NULL){
			list_destroy(list);
		}
	}

     /*Libertar memÃ³ria da tabela. */
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

/* Esta Ã© dada! Ao estilo C! */
int table_size(struct table_t *table) {
	/*ATENÇÃO QUE NÂO PODE SER O SIZE (PEDRO)*/
	return table == NULL ? -1 : table->nListas;
}

char **table_get_keys(struct table_t *table) {

}

void table_free_keys(char **keys) {

}

