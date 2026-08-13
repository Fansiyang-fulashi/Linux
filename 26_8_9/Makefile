BIN=exe
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

$(BIN):$(OBJ)
	@gcc -o $@ $^ -g
%.o:%.c
	@gcc -c $< -g

.PHONY:clean
clean:
	@rm -f $(BIN) $(OBJ)

