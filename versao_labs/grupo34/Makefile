# Sistemas Distribuidos - 2016 - Grupo 34
# Elias Miguel Barreira 40821, Pedro Pais 41375
# Silvia Ferreira 45511

CFLAGS = -g -pthread -Wall -I. -I./include
CC = gcc

all: test_data test_entry test_list test_table test_message table_client table_server

test_data: data.o

test_entry: data.o entry.o

list_objects = data.o entry.o list.o list-private.o
test_list: $(list_objects)

table_objects = $(list_objects) table.o table-private.o
test_table: $(table_objects)

message_objects = $(list_objects) message.o message-private.o
test_message: $(message_objects)

table_client: network_client.o $(message_objects) client_stub.o

table_server: table_server-private.o table_skel.o network_client.o client_stub.o $(table_objects) $(message_objects)

clean:
	rm -f *.o test_data test_entry test_list test_table test_message table_client table_server
