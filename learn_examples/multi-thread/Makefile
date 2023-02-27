CC=gcc

EXAMPLES= multi-hello shm-test stack-probe alipay

default:
	@echo "Usage: make [the example you want to run]"

$(EXAMPLES): %: %.out
	@./$@.out

%.out: %.c
	$(CC) $*.c -lpthread -o $@

clean:
	rm -rf *.out *.o

PHONY: clean



