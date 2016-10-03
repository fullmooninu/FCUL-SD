/* TODO PUT HEADER HERE */

/* Cria uma nova lista. Em caso de erro, retorna NULL.
 */
struct list_t *list_create() {
	//PARA COMPLETAR
	struct list_t *l;
	list_t -> head = NULL;
	return head;
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

}

/* Obtem um elemento da lista que corresponda à chave key. 
 * Retorna a referência do elemento na lista (ou seja, uma alteração
 * implica alterar o elemento na lista). 
 */
struct entry_t *list_get(struct list_t *list, char *key) {

}

/* Retorna o tamanho (numero de elementos) da lista 
 * Retorna -1 em caso de erro.  */
int list_size(struct list_t *list) {

}

/* Devolve um array de char * com a cópia de todas as keys da 
 * tabela, e um último elemento a NULL.
 */
char **list_get_keys(struct list_t *list) {

}

/* Liberta a memoria reservada por list_get_keys.
 */
void list_free_keys(char **keys) {
	
}
