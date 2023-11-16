all: mem_test

mem_test: mem_test.o my_malloc.o
	g++ -Wall -o  mem_test mem_test.o  my_malloc.o -lpthread

mem_test.o: mem_test.c my_malloc.h
	g++ -Wall -o mem_test.o -c mem_test.c
	

my_malloc.o:  my_malloc.c  my_malloc.h 
	g++ -Wall -o  my_malloc.o  -c  my_malloc.c
	
# list.o: list.h list.c
# 	g++ -Wall -c list.c
	
clean:
	rm -f  mem_test main.o 
