CC = g++
CFLAGS = -Wall -O3 -std=c++11

TARGET = esercizio3.1.exe
OBJS = esercizio3.1.o random.o

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

esercizio3.1.o: esercizio3.1.cpp funzioni.h data_analysis.h
	$(CC) -c esercizio3.1.cpp $(CFLAGS)

random.o: random.cpp random.h
	$(CC) -c random.cpp $(CFLAGS)

clean:
	rm -f *.o $(TARGET) seed.out