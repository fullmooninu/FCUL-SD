/* TODO PUT HEADER HERE */

/* Cria uma nova lista. Em caso de erro, retorna NULL.
 */
struct list_t *list_create() {
	struct list_t *l;
	l = (struct list_t*) malloc(sizeof (struct list_t));
	if(l == NULL) return NULL;
	l -> head = l;
	l -> size = 0;
	return l;
}

/* Elimina uma lista, libertando *toda* a memoria utilizada pela
 * lista.
 */
void list_destroy(struct list_t *list) {

}

/* Adiciona uma entry na lista. Como a lista deve ser ordenada, 
 * a nova entry deve ser colocada no local correto.
 * Retorna 0 (OK) ou -1 (erro)
 */
int list_add(struct list_t *list, struct entry_t *entry) {
	//PARA COMPLETAR
	if (list == NULL || entry_t == NULL) return NULL;

	struct node_t *newNode = (struct node_t*) malloc(sizeof(struct node_t));

	newNode -> entry_t = entry;
	newNode -
	}
}

/* Elimina da lista um elemento com a chave key. 
 * Retorna 0 (OK) ou -1 (erro)
 */
int list_remove(struct list_t *list, char* key) {
	if(list == NULL || key == NULL) return -1;

	/* Apontador que vai percorrer os v·rios nÛs */
	struct node_t *current = head;
	while (strcmp(current -> entry -> key,*key) != 0)
		current = head.next;
	/* Se chegar ao fim e n„o estiver l· a chave, d· erro. */
	if(strcmp(current -> entry -> key, *key) != 0) return -1;

	entry_destroy(current); //TEM-SE QUE FAZER O INCLUDE DO ENTRY.H?
	return 0;
}

/* Obtem um elemento da lista que corresponda √† chave key. 
 * Retorna a refer√™ncia do elemento na lista (ou seja, uma altera√ß√£o
 * implica alterar o elemento na lista). 
 */
struct entry_t *list_get(struct list_t *list, char *key) {

}

/* Retorna o tamanho (numero de elementos) da lista 
 * Retorna -1 em caso de erro.  */
int list_size(struct list_t *list) {
	if (list == NULL || list->size < 0) return -1;
	return list -> size;
}

/* Devolve um array de char * com a c√≥pia de todas as keys da 
 * tabela, e um √∫ltimo elemento a NULL.
 */
char **list_get_keys(struct list_t *list) {

}

/* Liberta a memoria reservada por list_get_keys.
 */
void list_free_keys(char **keys) {
	
}
