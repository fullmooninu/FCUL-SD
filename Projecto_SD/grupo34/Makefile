# Sistemas Distribuidos - 2016 - Grupo 34
# Elias Miguel Barreira 40821, Pedro Pais 41375
# Silvia Ferreira 45511

CFLAGS = -g -Wall -I. -I./include
CC = gcc

all: test_data test_entry test_list test_table test_message table-client table-server table_skel

test_data: data.o

test_entry: data.o entry.o

list_objects = data.o entry.o list.o list-private.o
test_list: $(list_objects)

test_table: $(list_objects) table.o table-private.o

test_message: $(list_objects) message.o message-private.o

table-client: network_client.o message.o message-private.o $(list_objects) client_stub.o

table-server: table-server-private.o table.o message.o message-private.o $(list_objects)

table_skel:

clean:
	rm -f *.o test_data test_entry test_list test_table test_message table-client table-server client_stub table_skel
