BIN1=server
BIN2=client
SRC1=server.cpp
SRC2=client.cpp

.PHONY:all clean
all:$(BIN1) $(BIN2)
$(BIN1):$(SRC1)
	g++ -o $@ $^

$(BIN2):$(SRC2)
	g++ -o $@ $^

clean:
	rm -f $(BIN1) $(BIN2)

