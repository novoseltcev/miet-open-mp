CC=clang++
CFLAGS= -std=c++20 -fopenmp

app: AutoTest.out
	$(CC) -o app.out AutoTest.out src/main.cpp  $(CFLAGS) 
	
AutoTest.out: src/AutoTest.h
	$(CC) -o AutoTest.out -c src/AutoTest.cpp $(CFLAGS)

clean :
	rm *.out > /dev/null
