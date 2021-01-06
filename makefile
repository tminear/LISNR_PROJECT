CFLAGS = -Wall -std=c++14

build: main.o encoder.o
	g++ $(CFLAGS) -lcurl main.o encoder.o -o encoder

main.o: main.cpp encoder.h
	g++ -c $(CFLAGS) main.cpp

encoder.o: encoder.cpp encoder.h
	g++ -c $(CFLAGS) encoder.cpp

clean:
	rm -f *.o

tidy:
	make clean
	rm encoder

archive:
	make clean
	tar czvf archive.tgz .
