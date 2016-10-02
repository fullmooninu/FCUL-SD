#include "projeto1_headers/data.h"
#include <stdlib.h>
#include <string.h>


//#define NULL 0

/* TODO FINISH HEADER Sistemas Distribuidos - 2016 - Grupo 34 - Proj1:
Elias Miguel Barreira 40821, Pedro Pais 4XXXX
Silvia Ferreira 45511 */

/* Funcao que cria um novo elemento de dados data_t e reserva a memoria
 * necessaria, especificada pelo parametro size
 */
struct data_t *data_create(int size) {
	struct data_t *retVal;
	if (size <= 0) return NULL;
	//Alocacao da estrutura do bloco de dados
	retVal = (struct data_t*) malloc(sizeof(struct data_t));
	if (retVal == NULL) return NULL;
	//define o size
	retVal->datasize = size;
	//Aloca espaco para a estrutura que o ponteiro vai apontar,
	//free, se falhar
	retVal->data = malloc(size);
	if (retVal->data == NULL) {
		free(retVal);
		return NULL;
	}
	return retVal;
}
/* Função idêntica à anterior, mas que inicializa os dados de acordo com
 * o parâmetro data.
 */
struct data_t *data_create2(int size, void *data) {
	struct data_t *retVal;
	if (data == NULL) return NULL;
	retVal = data_create(size);
	if (retVal == NULL) return NULL;
	memcpy(retVal->data, data, size);
	return retVal;
}

/* Função que destrói um bloco de dados e liberta toda a memória.
 */
void data_destroy(struct data_t *data2) {
	if(data2 != NULL){
	free(data2->data);
	free(data2);
}
}
/* Função que duplica uma estrutura data_t.
 */
struct data_t *data_dup(struct data_t *data) {
	if(data == NULL) return NULL;
	struct data_t *copia;
	copia = data_create2(data->datasize, data->data);
	return copia;
}
