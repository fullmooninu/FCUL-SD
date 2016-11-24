/* Sistemas Distribuidos - 2016 - Grupo 34
Elias Miguel Barreira 40821, Pedro Pais 41375
Silvia Ferreira 45511 */

#ifndef _PRIMARY_BACKUP_PRIVATE_H
#define _PRIMARY_BACKUP_PRIVATE_H

#include "primary_backup.h"
#include "table.h"

//TODO O QUE DEVE CONTER ISTO?
struct server_t {
  char* address;
  int port;
  int status; // PRIMARY = 0; SECONDARY = 1;
};

//TODO DEFINE NEW MESSAGE.H OPCODES FOR SERVER SPECIFIC OPERATIONS:
//ACTUALIZAR TABELA -> TABELA ACTUALIZADA
//BECOME PRIMARY -> IS NOW PRIMARY

#endif