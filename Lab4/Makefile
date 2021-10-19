CC=clang++
CFLAGS= -std=c++20 -fopenmp

all: app test

app: 
	$(CC) src/main.cpp -o app.out $(CFLAGS) 
	
test: 
	$(CC) src/test.cpp -o test.out $(CFLAGS) 

clean :
	rm *.out > /dev/null
