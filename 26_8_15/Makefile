SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
BIN=exe

$(BIN):$(OBJ)
	gcc -o $@ $^
%.o:%.c
	gcc -c $<

.PHONY:clean
clean:
	rm -f $(BIN) $(OBJ)

