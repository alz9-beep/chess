CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -g
SRC = src/main.c src/board.c src/movegen.c src/utils.c
OBJ = $(SRC: .c=.o)
TARGET = chess_game

all: $(TARGET)

$(TARGET):$(OBJ) 
	$(CC) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET)

