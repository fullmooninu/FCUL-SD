# Sistemas Distribuidos - 2016 - Grupo 34 - Proj2:
# Elias Miguel Barreira 40821, Pedro Pais 41375
# Silvia Ferreira 45511

CFLAGS = -g -Wall -I./include
CC = gcc

all: test_data test_entry test_list test_table test_message

list_objects = data.o entry.o list.o list-private.o
test_table: $(list_objects) table.o table-private.o

test_message: message.o message-private.o

test_list: data.o entry.o list.o list-private.o

test_entry: data.o entry.o

test_data: data.o


clean:
	rm -f *.o test_data test_entry test_list test_table test_message
