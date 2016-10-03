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
	struct node* current = list -> head;
	struct node* next;
	while (current != NULL) {
		next = current -> next;
		entry_destroy(current -> entry)
		free(current -> entry)
		free(current);
		current = next;
	};
	free(list -> head);
	list -> *head = NULL;
	*list = NULL;
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
	list_t -> size++;
	sort(list_t *list);
}

/* Elimina da lista um elemento com a chave key. 
 * Retorna 0 (OK) ou -1 (erro)
 */
int list_remove(struct list_t *list, char* key) {
	//FALTA COMPLETAR
	if(list == NULL || key == NULL) return -1;
	/* Apontador que vai percorrer os vários nós */
	struct node_t *current = head;
	while (strcmp(current -> entry -> key,*key) != 0)
		current = head.next;
	/* Se chegar ao fim e não estiver lá a chave, dá erro. */
	if(strcmp(current -> entry -> key, *key) != 0) return -1;

	entry_destroy(current); //TEM-SE QUE FAZER O INCLUDE DO ENTRY.H?
	//FALTA APAGAR NÒ E JUNTAR OS NEXT

	return 0;
}


/* Obtem um elemento da lista que corresponda Ã  chave key. 
 * Retorna a referÃªncia do elemento na lista (ou seja, uma alteraÃ§Ã£o
 * implica alterar o elemento na lista). 
 */
struct entry_t *list_get(struct list_t *list, char *key) {
	if (list == NULL || key == NULL) return NULL;
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

/* Devolve um array de char * com a cÃ³pia de todas as keys da 
 * tabela, e um Ãºltimo elemento a NULL.
 */
char **list_get_keys(struct list_t *list) {
	// criar array com o size da lista
	// para cada elemento da lista:
	// array[i] node -> entry -> key )
	struct node* current = list_t -> head;
	**char list_keys[list_t -> size + 1]; 
	int i;
	for (i = 0; i < list_t -> size, i++) {
		list_keys[i] = node -> entry -> key;
		current = current -> next;
	}
	list_keys[list_t -> size] = NULL;
	free(i);
	return list_keys;
}

/* Liberta a memoria reservada por list_get_keys.
 */
void list_free_keys(char **keys) {
	free(keys);
}
