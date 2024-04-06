# Compiler
CC = gcc
# Compiler flags
CFLAGS = -lstdc++ -Wall -Werror -g -fsanitize=address 
# Source files
SOURCES = 2.cpp
# Object files
OBJECTS = $(SOURCES:.cpp=.o)
# Executable name
EXECUTABLE = ../bin/app2

# Default target
all: $(EXECUTABLE)

# Linking the executable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

# Compiling source files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJECTS)
