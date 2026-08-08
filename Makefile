# Makefile - Terminal21 (Trabalho Final ED1)

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = blackjack

SRC = cartas.c mao.c deck.c turno.c eventos.c main.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c *.h
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET) $(TARGET).exe

.PHONY: all run clean