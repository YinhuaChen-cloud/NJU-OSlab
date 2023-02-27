CC=gcc

BIN=cyhpstree

default: $(BIN) 
	@./$(BIN)

$(BIN): %: %.c
	$(CC) $*.c -o $@

clean:
	rm -rf *.out *.o $(BIN)

PHONY: clean



