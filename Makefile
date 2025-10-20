CC = gcc

CFLAGS = -Wall -Wextra -Iinclude
SRC = src/main.c src/parser.c  src/util.c src/build.c
OUT = ember

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)