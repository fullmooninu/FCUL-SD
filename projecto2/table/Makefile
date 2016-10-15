#/* Sistemas Distribuidos - 2016 - Grupo 34 - Proj1:
#Elias Miguel Barreira 40821, Pedro Pais 41375
#Silvia Ferreira 45511 */

FLAGS = -Wall
OBJS_TEST_DATA = data.o test_data.o entry.o
OBJS_TEST_ENTRY = data.o entry.o test_entry.o
OBJS_TEST_LIST = data.o entry.o list.o list-private.o test_list.o
OBJS_TEST_TABLE = data.o entry.o list.o list-private.o table.o table-private.o test_table.o

all: testData testEntry testList testTable

testData: ${OBJS_TEST_DATA}
	gcc -g ${FLAGS} -o testData ${OBJS_TEST_DATA}

testEntry: ${OBJS_TEST_ENTRY}
	gcc -g ${FLAGS} -o testEntry ${OBJS_TEST_ENTRY}

testList: ${OBJS_TEST_LIST}
	gcc -g ${FLAGS} -o testList ${OBJS_TEST_LIST}

testTable: ${OBJS_TEST_TABLE}
	gcc -g ${FLAGS} -o testTable ${OBJS_TEST_TABLE}

data.o: data.c include/data.h
	gcc -g -c ${FLAGS} data.c

test_data.o: test_data.c include/data.h
	gcc -g -c ${FLAGS} test_data.c

entry.o: entry.c include/data.h include/entry.h
	gcc -g -c ${FLAGS} entry.c

test_entry.o: test_entry.c include/data.h include/entry.h
	gcc -g -c ${FLAGS} test_entry.c

list.o: list.c list-private.c include/list.h include/list-private.h include/entry.h include/data.h
	gcc -g -c ${FLAGS} list.c

list-private.o: list-private.c include/list-private.h include/list.h include/entry.h
	gcc -g -c ${FLAGS} list-private.c

table.o: table.c include/table.h include/table-private.h
	gcc -g -c ${FLAGS} table.c

table-private.o: table-private.c include/table-private.h include/table.h
	gcc -g -c ${FLAGS} table-private.c

test_list.o: test_list.c include/list-private.h include/list.h include/entry.h include/data.h
	gcc -g -c ${FLAGS} test_list.c

test_table.o: test_table.c include/table-private.h
	gcc -g -c ${FLAGS} test_table.c

clean:
	rm -f *.o testData testEntry testList testTable
