CC=gcc

default:
	@echo "do nothing"

multi-hello:
	$(CC) $@.c -lpthread	&& ./a.out

shm-test:
	$(CC) $@.c -lpthread	&& ./a.out

stack-probe:
	$(CC) $@.c -lpthread	&& ./a.out

clean:
	rm -rf a.out *.o

PHONY: clean



