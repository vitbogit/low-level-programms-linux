CC = gcc
CFLAGS = -lstdc++ -Wall -fsanitize=address -lreadline -o

OUTPUT = app
SOURCES = $(wildcard ./src/*.c)

TEST_OUTPUT = testapp
TEST_SOURCES = $(wildcard ./test/*.c) $(filter-out ./src/main.c, $(wildcard ./src/*.c))

all:
	$(CC) $(CFLAGS) $(OUTPUT) $(SOURCES) 
	$(CC) $(CFLAGS) $(TEST_OUTPUT) $(TEST_SOURCES) 


