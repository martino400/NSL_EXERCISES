CC = g++
CFLAGS = -O3 -std=c++14 -I/opt/homebrew/Cellar/armadillo/15.2.4/include
LDFLAGS = -L/opt/homebrew/Cellar/armadillo/15.2.4/lib -larmadillo

all: simulator.exe

# Collegamento dei file oggetto per creare l'eseguibile
simulator.exe: TSP.o simulator.o random.o
	$(CC) TSP.o random.o simulator.o -o simulator.exe $(LDFLAGS)

# CORREZIONE: cambiato simulator.c++ in simulator.cpp
simulator.o: simulator.cpp TSP.h
	$(CC) -c simulator.cpp -o simulator.o $(CFLAGS)

TSP.o: TSP.cpp TSP.h random.h
	$(CC) -c TSP.cpp -o TSP.o $(CFLAGS)

random.o: random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)

clean:
	rm -f *.o simulator.exe

remove:
	rm -f ../OUTPUT/*.*
	rm -f ../OUTPUT/CONFIG/*.*