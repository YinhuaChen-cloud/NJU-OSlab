# gcc -c minimal.c && objdump -d minimal.o && ld minimal.o
default:
	gcc minimal.S -c && ld minimal.o
	./a.out

clean:
	rm -rf a.out *.o

PHONY: clean
