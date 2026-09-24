CC = g++
CFLAGS = -Wall -O3 --std=c++11

esercizio2.2.exe : esercizio2.2.o random.o funzioni.h RandomWalk.o
	$(CC) RandomWalk.o random.o esercizio2.2.o -o esercizio2.2.exe
esercizio2.2.o : esercizio2.2.cpp
	$(CC) -c esercizio2.2.cpp -o esercizio2.2.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
RandomWalk.o : RandomWalk.cpp RandomWalk.h
	$(CC) -c RandomWalk.cpp -o RandomWalk.o $(CFLAGS)
clean :
	rm *.o esercizio2.2.exe seed.out
