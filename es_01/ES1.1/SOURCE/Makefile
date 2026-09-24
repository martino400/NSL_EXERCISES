CC = g++
CFLAGS = -Wall -O3 --std=c++11

esercizio1.0.exe : esercizio1.0.o random.o funzioni.h
	$(CC) random.o esercizio1.0.o -o esercizio1.0.exe
esercizio1.0.o : esercizio1.0.cpp
	$(CC) -c esercizio1.0.cpp -o esercizio1.0.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
clean :
	rm *.o esercizio1.0.exe seed.out
