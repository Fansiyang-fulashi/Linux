SRCPP=$(wildcard *.cpp)
BIN=exe

$(BIN):$(SRCPP)
	g++ -o $@ $^

.PHONY:clean
clean:
	rm -f $(BIN)
