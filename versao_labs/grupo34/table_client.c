/* Sistemas Distribuidos - 2016 - Grupo 34
 Elias Miguel Barreira 40821, Pedro Pais 41375
 Silvia Ferreira 45511 */

/*
 Programa cliente para manipular tabela de hash remota.
 Os comandos introduzido no programa não deverão exceder
 80 carateres.

 Uso: table-client <ip servidor>:<porta servidor> <ip servidor>:<porta servidor secundario>
 Exemplo de uso: ./table_client 10.101.148.144:54321 10.101.148.144:12345
 */

#include "network_client-private.h"
#include "client_stub-private.h"

int main(int argc, char **argv) {
	struct rtable_t *server = (struct rtable_t*)malloc(sizeof(struct rtable_t));
	//struct rtable_t *server_backup = NULL;
	char *key, *data = NULL, **keys = NULL;
	struct data_t *datat = NULL;

	/* Testar os argumentos de entrada */
	if (argc != 3) {
		printf("Número de argumentos inválido.\n");
		return -1;
	}

	/* Usar network_connect para estabelcer ligação ao servidor */
	server = rtable_bind(argv[1]);
	if (server == NULL) {
		printf("A tentar novo servidor: de %s para %s\n", argv[1], argv[2]);
		sleep(RETRY_TIME);
		//Faz ligação ao segundo servidor
		server = rtable_bind(argv[2]);
		if (server != NULL)
			server->on1 = 0;
		if (server == NULL) {
			server = rtable_bind(argv[2]);
			printf("A tentar novo servidor: de %s para %s\n", argv[2], argv[1]);
			sleep(RETRY_TIME);
			//Faz ligação ao primeiro servidor
			server = rtable_bind(argv[1]);
			if (server != NULL)
				server->on1 = 1;
		}
	}
	//Se nenhum funcionar
	if (server == NULL) {
		perror("Impossível ligar a um servidor");
		return -1;
	}

	printf("\nConectou-se ao servidor\n");

	/* Fazer ciclo até que o utilizador resolva fazer "quit" */
	while (1) {
		char input[81], *s;
		int result;
		key = NULL;
		data = NULL;
		keys = NULL;
		datat = NULL;

		// Mostrar a prompt para inserção de comando

		printf("\n\nEscolha um dos comandos abaixo:\n");
		printf(
				" put <key> <data>\n get <key>\n update <key> <data>\n del <key>\n size\n quit\n\n");
		printf(">>> ");

		/* Receber o comando introduzido pelo utilizador
		 Sugestão: usar fgets de stdio.h
		 Quando pressionamos enter para finalizar a entrada no
		 comando fgets, o carater \n é incluido antes do \0.
		 Convém retirar o \n substituindo-o por \0.
		 */
		fgets(input, 80, stdin);
		s = strchr(input, '\n');
		*s = '\0';

		if (strcmp(input, "quit") == 0) {
			return rtable_unbind(server);
		}

		if (strncmp(input, "put ", 4) == 0) {
			printf("Comando PUT\n");

			strtok(input, " "); //ignorar o "comando"
			key = strtok(NULL, " ");
			if (key == NULL) {
				printf("Comando com formato inválido.\n");
				continue;
			}
			data = strtok(NULL, ""); //le ate ao fim da linha 'input'
			if (data == NULL) {
				printf("Comando com formato inválido.\n");
				continue;
			}

			datat = data_create2(strlen(data), data);
			if (datat == NULL) {
				rtable_unbind(server);
				return -1;
			}
			result = rtable_put(server, key, datat);

			printf("PUT result: ");
			if (result == -1) {
				printf("ERRO\n");
			} else {
				printf("OK\n");
			}

			data_destroy(datat);

		} else if (strncmp(input, "get ", 4) == 0) {
			printf("Comando GET\n");

			strtok(input, " "); //ignorar o "comando"
			key = strtok(NULL, ""); //le ate ao fim da linha 'input'
			if (key == NULL) {
				printf("Comando com formato inválido.\n");
				continue;
			}

			if (strcmp(key, "*") == 0) {
				keys = rtable_get_keys(server);
				if (keys != NULL && keys[0] != NULL) {
					printf("GET result:\n");
					int i;
					for (i = 0; keys[i] != NULL; i++) {
						printf("key[%d]: %s\n", i, keys[i]);
					}
				} else {
					printf("GET result: NO KEYS\n");
				}
				rtable_free_keys(keys);
			} else {
				datat = rtable_get(server, key);

				printf("GET result: ");
				if (datat != NULL) {
					printf("\n");
					print_data(datat);
				} else {
					printf("ERRO\n");
				}
			}

			data_destroy(datat);

		} else if (strncmp(input, "update ", 7) == 0) {
			printf("Comando UPDATE\n");

			strtok(input, " "); //ignorar o "comando"
			key = strtok(NULL, " ");
			if (key == NULL) {
				printf("Comando com formato inválido.\n");
				continue;
			}
			data = strtok(NULL, ""); //le ate ao fim da linha 'input'
			if (data == NULL) {
				printf("Comando com formato inválido.\n");
				continue;
			}

			datat = data_create2(strlen(data), data);
			if (datat == NULL) {
				rtable_unbind(server);
				return -1;
			}
			result = rtable_update(server, key, datat);

			printf("UPDATE result: ");
			if (result == -1) {
				printf("ERRO\n");
			} else {
				printf("OK\n");
			}

			data_destroy(datat);

		} else if (strncmp(input, "del ", 4) == 0) {
			printf("Comando DEL\n");

			strtok(input, " "); //ignorar o "comando"
			key = strtok(NULL, ""); //le ate ao fim da linha 'input'
			if (key == NULL) {
				printf("Comando com formato inválido.\n");
				continue;
			}

			result = rtable_del(server, key);

			printf("DEL result: ");
			if (result == -1) {
				printf("ERRO\n");
			} else {
				printf("OK\n");
			}

		} else if (strncmp(input, "size", 4) == 0) {
			printf("Comando SIZE\n");

			result = rtable_size(server);

			printf("SIZE result: ");
			if (result == -1) {
				printf("ERRO\n");
			} else {
				printf("%d\n", result);
			}

		} else {
			printf("Comando desconhecido.\n");
		}

	} //while
	return rtable_unbind(server);
}
