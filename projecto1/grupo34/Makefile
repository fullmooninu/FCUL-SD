FLAGS = -Wall
TARGET = data
OBJS = data.o test_data.o
#HEADERS = projeto1_headers

all: ${TARGET}

${TARGET}: ${OBJS}
	gcc -g ${FLAGS} -o ${TARGET} ${OBJS}

data.o: data.c projeto1_headers/data.h
	gcc -g -c ${FLAGS} data.c

test_data.o: test_data.c projeto1_headers/data.h
	gcc -g -c ${FLAGS} test_data.c


clean:
	rm -f *.o ${TARGET}
