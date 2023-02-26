CC=gcc

EXAMPLES= multi-hello shm-test stack-probe

default:
	@echo "do nothing"

$(EXAMPLES): 
	$(CC) $@.c -lpthread	&& ./a.out

clean:
	rm -rf a.out *.o

PHONY: clean



