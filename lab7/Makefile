FLAGS = -Wall -g -std=gnu99

# all: library.cpp main.cpp
# $@ is all, or the name of the file to be created(i.e. the target)
# $< is library.cpp, usually the source file
# $^ is library.cpp main.cpp
starter : simplefork forkloop

all : simplefork forkloop parentcreates childcreates

simplefork : simplefork.o
	gcc ${FLAGS} -o simplefork simplefork.o

forkloop : forkloop.o
	gcc ${FLAGS} -o $@ $^

parentcreates : parentcreates.o
	gcc ${FLAGS} -o $@ $^

childcreates : childcreates.o
	gcc ${FLAGS} -o $@ $^

%.o : %.c
	gcc ${FLAGS} -c $<

clean :
	rm *.o simplefork forkloop

clean-all :
	rm *.o simplefork forkloop parentcreates childcreates
