/* TODO FINISH HEADER Sistemas Distribuidos - 2016 - Grupo 34 - Proj1:
Elias Miguel Barreira 40821, Pedro Pais 4XXXX 
Silvia Ferreira 4XXXX */

struct data_t *datacreate(int size) {
	if size (<= 0) return NULL;
	struct data_t *a;
	a = (struct data_t *) malloc(sizeof struct data_t);
	if (a == NULL) return NULL;
	a -> datasize = size;
	a -> data = malloc(size);
	if ( a -> data = NULL) {
		free(a);
		return NULL;
	}
	return a;
}

struct data_t *datacreate2(int size, void *data) {
	if (data == NULL) return NULL;
	struct data_t *a = datacreate(size);
	if (a == NULL) return NULL;
	memcpy (a -> data, data, size);
	return a;
}


struct data_destroy void data_destroy(struct data_t *data) {
	// TODO feito na aula, nao cheguei a passar ~ miguel
	RETURN NULL;
}

struct data_t *datadup (struct data_t *data) {
	if (data == NULL) return NULL;
	struct data_t *copia = datacreate2 (data-> datasize, data-> data);
	return copia;
}