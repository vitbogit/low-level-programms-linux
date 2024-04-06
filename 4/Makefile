# Compiler
CC = gcc
# Compiler flags
CFLAGS = -lstdc++ -lpthread -lm -Wall -fsanitize=address 
# Source files
SOURCES = sobel_multithread.cpp
# Object files
OBJECTS = $(SOURCES:.cpp=.o)
# Executable name
EXECUTABLE = sobel_multithread

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
