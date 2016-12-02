/* Sistemas Distribuidos - 2016 - Grupo 34
Elias Miguel Barreira 40821, Pedro Pais 41375
Silvia Ferreira 45511 */

/*
Programa que implementa um servidor de uma tabela hash com chainning.
Uso: table-server <porta TCP> <dimensão da tabela>
Exemplo de uso: ./table_server 54321 10
*/
#include <error.h>
#include <poll.h>
#include <fcntl.h>
#include "inet.h"
#include "table-private.h"
#include "message-private.h"
#include "table_server-private.h"
#include "table_skel.h"
//#include "message.h"
#define MAX_CLIENTES 1

#define NFDESC 4 // N�mero de sockets (uma para listening)
#define TIMEOUT 50 // em milisegundos

/* Função para preparar uma socket de receção de pedidos de ligação.
*/
int make_server_socket(short port){
  int socket_fd;
  struct sockaddr_in server;

  if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
    perror("Erro ao criar socket");
    return -1;
  }

  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  server.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(socket_fd, (struct sockaddr *) &server, sizeof(server)) < 0){
    perror("Erro ao fazer bind");
    close(socket_fd);
    return -1;
  }

  if (listen(socket_fd, 0) < 0){
    perror("Erro ao executar listen");
    close(socket_fd);
    return -1;
  }
  return socket_fd;
}




/* Função "inversa" da função network_send_receive usada no table-client.
Neste caso a função implementa um ciclo receive/send:

Recebe um pedido;
Aplica o pedido na tabela;
Envia a resposta.#include "message.h"

*/
int network_receive_send(int sockfd){
  char *message_resposta = NULL;
  char *message_pedido = NULL;
  //int msg_length;
  int host_size, net_size, result;
  struct message_t *msg_pedido = NULL;
  struct message_t *msg_resposta = NULL;
  //int msg_length;
  //struct list_t *results;

  /* Verificar parâmetros de entrada */

  if(sockfd < 0) return -1;


  /* Com a função read_all, receber num inteiro o tamanho da
  mensagem de pedido que será recebida de seguida.*/
  result = read_all(sockfd, (char *) &net_size, _INT);
  /* Verificar se a receção teve sucesso */
  if (result == -1) return -1;

  host_size = ntohl(net_size);

  /* Alocar memória para receber o número de bytes da
  mensagem de pedido. */
  message_pedido = malloc(sizeof(char)*host_size);

  /* Com a função read_all, receber a mensagem de resposta. */
  result = read_all(sockfd, message_pedido, host_size);
  // printf("bytes recebidos buffer: %d\n", result);
  // fflush(stdout);

  /* Verificar se a receção teve sucesso */
  if (result == -1){
    free_memory(message_resposta, message_pedido, msg_pedido, msg_resposta);
    return -1;
  }

  /* Desserializar a mensagem do pedido */
  msg_pedido = buffer_to_message(message_pedido, host_size);

  /* Verificar se a desserialização teve sucesso */
  if (msg_pedido == NULL){
    free_memory(message_resposta, message_pedido, msg_pedido, msg_resposta);
    return -1;
  }

  /* Processar a mensagem */
  msg_resposta = invoke(msg_pedido);

  /* Serializar a mensagem recebida */
  host_size = message_to_buffer(msg_resposta, &message_resposta);

  /* Verificar se a serialização teve sucesso */
  if (host_size == -1){
    free_memory(message_resposta, message_pedido, msg_pedido, msg_resposta);
    return -1;
  }

  /* Enviar ao cliente o tamanho da mensagem que será enviada
  logo de seguida
  */
  net_size = htonl(host_size);
  result = write_all(sockfd, (char *) &net_size, _INT);

  /* Verificar se o envio teve sucesso */
  if (result == -1){
    free_memory(message_resposta, message_pedido, msg_pedido, msg_resposta);
    return -1;
  }

  /* Enviar a mensagem que foi previamente serializada */

  result = write_all(sockfd, message_resposta, host_size);

  /* Verificar se o envio teve sucesso */
  if (result == -1) {
    free_memory(message_resposta, message_pedido, msg_pedido, msg_resposta);
    return -1;
  }

  /* Libertar memória */
  free_memory(message_resposta, message_pedido, msg_pedido, msg_resposta);


  return 0;
}


int main(int argc, char **argv){
  struct pollfd connections[NFDESC]; // Estrutura para file descriptors das sockets das ligações
  int listening_socket, i, nfds, kfds;
  struct sockaddr_in client;
  socklen_t size_client;

  if (argc != 3){
    printf("Uso: ./server <porta TCP> <dimensão da tabela>\n");
    printf("Exemplo de uso: ./table-server 54321 10\n");
    return -1;
  }

  if ((listening_socket = make_server_socket(atoi(argv[1]))) < 0) {
    return -1;
  }

  //TODO
  table_skel_init(atoi(argv[2]));


  printf("Servidor à espera de dados\n");
  size_client = sizeof(struct sockaddr);

  for (i = 0; i < NFDESC; i++)
    connections[i].fd = -1;    // poll ignora estruturas com fd < 0

  connections[0].fd = listening_socket;  // Vamos detetar eventos na welcoming socket
  connections[0].events = POLLIN;  // Vamos esperar ligações nesta socket

  nfds = 1; // número de file descriptors

  // Retorna assim que exista um evento ou que TIMEOUT expire. * FUNÇÃO POLL *.
  while ((kfds = poll(connections, nfds, TIMEOUT)) >= 0) {// kfds == 0 significa timeout sem eventos
    if (kfds > 0){ // kfds é o número de descritores com evento ou erro
      if ((connections[0].revents & POLLIN) && (nfds < NFDESC))  // Pedido na listening socket ?
        if ((connections[nfds].fd = accept(connections[0].fd, (struct sockaddr *) &client, &size_client)) > 0){ // Ligação feita ?
          connections[nfds].events = POLLIN; // Vamos esperar dados nesta socket
          nfds++;
      }

      for (i = 1; i < nfds; i++) { // Todas as ligações
        if (connections[i].revents & POLLIN) { // Dados para ler ?

          if (network_receive_send(connections[i].fd) < 0) {
            close(connections[i].fd);
            connections[i].fd = -1;
            continue;
          }
        }
      }
    }
  }

  table_skel_destroy();

  for (i = 0; i < nfds; i++) {
    close(connections[i].fd);
  }
  return 0;
}
