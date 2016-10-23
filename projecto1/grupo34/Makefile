CFLAGS = -g -Wall -I./include
CC = gcc

all: test_data test_entry test_list

test_list: data.o entry.o list.o list-private.o

test_entry: data.o entry.o

test_data: data.o

clean:
	rm -f *.o test_data test_entry test_list
