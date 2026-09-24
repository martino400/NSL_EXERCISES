CC = g++
CFLAGS = -Wall -O3 --std=c++11

esercizio2.1.exe : esercizio2.1.o random.o funzioni.h
	$(CC) random.o esercizio2.1.o -o esercizio2.1.exe
esercizio2.1.o : esercizio2.1.cpp
	$(CC) -c esercizio2.1.cpp -o esercizio2.1.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
clean :
	rm *.o esercizio2.1.exe seed.out
