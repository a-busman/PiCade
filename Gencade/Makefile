CC=gcc
CFLAGS=-c -Wall -O3
LIBS=-lbcm2835
INCLUDES=-Iinclude
EXECUTABLE=Gencade

Gencade: obj/main.o obj/Gencade.o
	$(CC) obj/main.o obj/Gencade.o $(LIBS) -o bin/$(EXECUTABLE)

obj/main.o: src/main.c
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) src/main.c -o obj/main.o

obj/Gencade.o: src/Gencade.c
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) src/Gencade.c -o obj/Gencade.o

clean:
	rm obj/*.o; rm bin/$(EXECUTABLE)
