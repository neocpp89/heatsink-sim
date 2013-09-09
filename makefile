# Project: heatsink-sim.
CC = gcc
BIN = hssim
CFLAGS = -c -O3 -Wall -Wstrict-prototypes -pedantic -g -pg
LIB = -lrt -lm -pthread -lblas -llapack
LDFLAGS = $(LIB) -pg -g
SRC = \
	main.c \
	logger.c \
	text_reader.c

OBJ = $(SRC:.c=.o)

.PHONY: clean
all: $(BIN)

clean:
	rm $(OBJ); \
	rm $(BIN);

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

