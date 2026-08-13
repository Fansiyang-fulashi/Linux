BIN=proc.exe
CC=gcc
LFLAGS=-o
FLAGS=-c
SRC=$(shell ls *.c)
RM=rm -f
OBJ=$(SRC:.c=.o)

$(BIN):$(OBJ)
	@$(CC) $(LFLAGS) $@ $^
%.o:%.c
	$(CC) $(FLAGS) $<
	@echo "linking $^ to $@"
.PHONY:clean
clean:
	@$(RM) $(BIN) $(OBJ)
	@echo "clean $(BIN) $(OBJ)"
.PHONY:test
test:
	@echo $(BIN)
	@echo $(SRC)
