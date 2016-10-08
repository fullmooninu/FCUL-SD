FLAGS = -Wall
OBJS_TEST_DATA = data.o test_data.o entry.o
OBJS_TEST_ENTRY = data.o entry.o test_entry.o
OBJS_TEST_LIST = list.o list-private.o test_list.o

all: testData testEntry testList

testData: ${OBJS_TEST_DATA}
	gcc -g ${FLAGS} -o testData ${OBJS_TEST_DATA}

testEntry: ${OBJS_TEST_ENTRY}
	gcc -g ${FLAGS} -o testEntry ${OBJS_TEST_ENTRY}

testList: ${OBJS_TEST_LIST}
		gcc -g ${FLAGS} -o testList ${OBJS_TEST_LIST}

data.o: data.c include/data.h
	gcc -g -c ${FLAGS} data.c

test_data.o: test_data.c include/data.h
	gcc -g -c ${FLAGS} test_data.c

entry.o: entry.c include/data.h include/entry.h
	gcc -g -c ${FLAGS} entry.c

test_entry.o: test_entry.c include/data.h include/entry.h
	gcc -g -c ${FLAGS} test_entry.c

list.o: list.c list-private.c include/list.h include/list-private.h include/entry.h
		gcc -g -c ${FLAGS} list.c

list-private.o: list-private.c include/list-private.h include/list.h include/entry.h

test_list.o: test_list.c include/list-private.h
		gcc -g -c ${FLAGS} test_list.c


clean:
	rm -f *.o testData testEntry testList
