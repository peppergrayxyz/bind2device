CC=gcc
CFLAGS=-std=c99 -Wall -Werror -pedantic -D_FORTIFY_SOURCE=2 -O1 -nostartfiles -fpic -shared -ldl -D_GNU_SOURCE
SRC=src
BIN=build

SRCS=$(wildcard $(SRC)/*.c)
BINS=$(SRCS:$(SRC)/%.c=%)

all: $(BINS)

%: $(SRC)/%.c
	$(CC) $(CFLAGS) -o $(BIN)/$@.so $<

clean:
	$(RM) $(BIN)/*