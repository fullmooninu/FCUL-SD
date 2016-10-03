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

/* novo metodo para fazer sort a lista*/
void sort() {
	// TODO
}

/* Adiciona uma entry na lista. Como a lista deve ser ordenada, 
 * a nova entry deve ser colocada no local correto.
 * Retorna 0 (OK) ou -1 (erro)
 */
int list_add(struct list_t *list, struct entry_t *entry) {
	if (list == NULL || entry_t == NULL) return NULL;

	struct node_t *newNode = (struct node_t*) malloc(sizeof(struct node_t));

	newNode -> entry_t = entry;
	
	// point it to the old first node
	newNode -> next = list -> head;
	
	// point first to the new first
	list_t -> head = newNode;
	list_t -> size = list_t -> size + 1;
	sort(list_t *list);
}

/* Elimina da lista um elemento com a chave key. 
 * Retorna 0 (OK) ou -1 (erro)
 */
int list_remove(struct list_t *list, char* key) {

	struct node* current = list_t -> head;
	struct node* previous = NULL;~
	
	// se a lista estiver vazia
	if (list -> head == NULL) return -1;

	while(current->key != key) {
		if (current-> next == NULL) {
			return -1;
		}else{
			previous = current;
			current = current -> next;
	    }
	}
	// encontrou
	if (current == list -> head  {
		(list -> head) = (list -> head -> next);

	}else{
		previous -> next = current -> next;
	list_t -> size = list_t -> size - 1



/* Obtem um elemento da lista que corresponda à chave key. 
 * Retorna a referência do elemento na lista (ou seja, uma alteração
 * implica alterar o elemento na lista). 
 */
struct entry_t *list_get(struct list_t *list, char *key) {
	struct node* current = list -> head;

	if (list -> head == NULL) return NULL;

	// correr a lista
	while (current -> key != key) {
		//se for o ultimo node
		if (current -> next == NULL) {
			return NULL;
		}else{
			current = current -> next
		}
	}
	return current;
}

/* Retorna o tamanho (numero de elementos) da lista 
 * Retorna -1 em caso de erro.  */
int list_size(struct list_t *list) {
	if (list == NULL || list->size < 0) return -1;
	return list -> size;
}

/* Devolve um array de char * com a cópia de todas as keys da 
 * tabela, e um último elemento a NULL.
 */
char **list_get_keys(struct list_t *list) {
	// criar array com o size da lista
	// para cada elemento da lista:
	// array[i] node -> entry -> key )
	
	struct node* current = list_t -> head;
	**char list_keys[list_t -> size]; 
	int i;
	
	for (i = 0; i < list_t -> size, i++) {
		list_keys[i] = node -> entry -> key;
		current = current -> next;
	}
}

/* Liberta a memoria reservada por list_get_keys.
 */
void list_free_keys(char **keys) {
	
}
