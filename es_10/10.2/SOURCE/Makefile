CC = mpicxx
# CAMBIATO DA -std=c++14 A -std=c++17
CFLAGS = -O3 -std=c++17 -I/opt/homebrew/opt/armadillo/include
LDFLAGS = -L/opt/homebrew/opt/armadillo/lib -larmadillo

all: simulator.exe

simulator.exe: TSP.o simulator.o random.o
	$(CC) TSP.o random.o simulator.o -o simulator.exe $(LDFLAGS)

# AGGIORNATO: Aggiunto lib.h alle dipendenze di simulator.o
simulator.o: simulator.cpp TSP.h lib.h
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