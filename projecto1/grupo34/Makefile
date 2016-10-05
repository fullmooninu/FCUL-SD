FLAGS = -Wall
OBJS_TEST_DATA = data.o test_data.o entry.o
OBJS_TEST_ENTRY = data.o entry.o test_entry.o

all: testData testEntry

testData: ${OBJS_TEST_DATA}
	gcc -g ${FLAGS} -o testData ${OBJS_TEST_DATA}

testEntry: ${OBJS_TEST_ENTRY}
	gcc -g ${FLAGS} -o testEntry ${OBJS_TEST_ENTRY}

data.o: data.c projeto1_headers/data.h
	gcc -g -c ${FLAGS} data.c

test_data.o: test_data.c projeto1_headers/data.h
	gcc -g -c ${FLAGS} test_data.c

entry.o: entry.c projeto1_headers/data.h projeto1_headers/entry.h
	gcc -g -c ${FLAGS} entry.c

test_entry.o: test_entry.c projeto1_headers/data.h projeto1_headers/entry.h
	gcc -g -c ${FLAGS} test_entry.c

clean:
	rm -f *.o testData testEntry
