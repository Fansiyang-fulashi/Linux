SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
EXE=exe

$(EXE):$(OBJ)
	@gcc -o $@ $^
%.o:%.c
	@gcc -c $<

.PHONY:clean
clean:
	@rm -f $(EXE) $(OBJ)

