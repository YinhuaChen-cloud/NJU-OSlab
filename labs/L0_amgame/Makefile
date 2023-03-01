CXX=g++

BIN=cyhpstree

default: $(BIN) 
	@./$(BIN)

gdb: $(BIN) 
	gdb $(BIN)

$(BIN): %: %.cpp
	$(CXX) $*.cpp -g -o $@

clean:
	rm -rf *.out *.o $(BIN)

PHONY: clean



