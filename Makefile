.PHONY: build run clean

build: myHDD

myHDD: *.o
	gcc *.o -o myHDD -lm

*.o: *.h *.c
	gcc -Wall -c *.c

run: build
	./myHDD

clean:
	rm -f myHDD *.o 
