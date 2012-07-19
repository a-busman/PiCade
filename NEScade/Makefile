CC=gcc
CFLAGS=-c -Wall -O3
LIBS=-lbcm2835
INCLUDES=-Iinclude
EXECUTABLE=NEScade

NEScade: obj/main.o obj/NEScade.o
	$(CC) obj/main.o obj/NEScade.o $(LIBS) -o bin/$(EXECUTABLE)

obj/main.o: src/main.c
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) src/main.c -o obj/main.o

obj/NEScade.o: src/NEScade.c
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) src/NEScade.c -o obj/NEScade.o

clean:
	rm obj/*.o; rm bin/$(EXECUTABLE)
