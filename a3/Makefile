FLAGS = -Wall -g -std=gnu99
DEPENDENCIES = helper.h

all : psort mkwords

psort: psort.o helper.o
	gcc ${FLAGS} -o $@ $^

mkwords: mkwords.o
	gcc ${FLAGS} -o $@ $^ -lm

%.o: %.c ${DEPENDENCIES}
	gcc ${FLAGS} -c $<

clean:
	rm -f *.o helper psort mkwords
