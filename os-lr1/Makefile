CC = gcc
CFLAGS = -lstdc++ -Wall -fsanitize=address -o  
OUTPUT = app
SOURCES = $(wildcard ./src/*.c)

all:
	$(CC) $(CFLAGS) $(OUTPUT) $(SOURCES)