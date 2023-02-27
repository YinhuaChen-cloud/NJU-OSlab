CC=gcc

EXAMPLES= multi-hello shm-test stack-probe alipay

TARGET=

default:
	@echo "Usage: make run TARGET=[the example you want to run]"

%.out: %.c
	$(CC) $*.c -lpthread -o $@

run: $(addsuffix .out, $(TARGET))
	@./$<

clean:
	rm -rf *.out *.o

PHONY: clean



