BIN=exe
SRC=$(wildcard *.cpp)

$(BIN):$(SRC)
	g++ -o $@ $^

.PHONY:clean
clean:
	rm -f $(BIN)