EXEC := main
CC := gcc
SRC := $(shell find . -maxdepth 2 -name "*.c" -not -path "./tests/*")
OBJ = $(patsubst %.c, %.o, $(SRC))

$(EXEC): $(OBJ)
	$(CC) $(LDFLAGS) -o $(EXEC) $(OBJ) $(LDLIBS)
	rm -f $(OBJ)

clean:
	rm -f $(OBJ)