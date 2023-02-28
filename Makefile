all: main.c writer.o reader.o
	gcc -Wall -O0 -o readersWriters main.c writer.o reader.o -lpthread

writer.o: writer.h writer.c
	gcc -Wall -O0 -c writer.c

reader.o: reader.h reader.c
	gcc -Wall -O0 -c reader.c

clean:
	rm readersWriters *.o
