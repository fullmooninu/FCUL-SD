CFLAGS = -g -Wall -I. -I./include
CC = gcc

all: test_data test_entry test_list

test_list: data.o entry.o list.o list-private.o

test_list.o: 

list.o:

list_private:

test_entry: data.o entry.o

test_entry.o:

entry.o:

test_data: data.o

test_data.o:

data.o:

clean:
	rm -f *.o test_data test_entry test_list
