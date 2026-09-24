CC = g++
CFLAGS = -O3 -std=c++14 -I$(shell brew --prefix armadillo)/include
LDFLAGS = -L$(shell brew --prefix armadillo)/lib -larmadillo


all: simulator.exe

simulator.exe: variation_mc.o variation_qm.o random.o
	$(CC) variation_mc.o random.o variation_qm.o -o simulator.exe $(LDFLAGS)

variation_qm.o: variation_qm.c++ variation_mc.h
	$(CC) -c variation_qm.c++ -o variation_qm.o $(CFLAGS)

variation_mc.o: variation_mc.cpp variation_mc.h random.h
	$(CC) -c variation_mc.cpp -o variation_mc.o $(CFLAGS)

random.o: random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)

clean:
	rm -f *.o simulator.exe

remove:
	rm -f ../OUTPUT/*.*
	rm -f ../OUTPUT/CONFIG/*.*