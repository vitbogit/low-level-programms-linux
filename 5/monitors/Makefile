# Compiler
CC = gcc
# Compiler flags
CFLAGS = -lstdc++ -lm -Wall -fsanitize=address
# Source files
SOURCES = bathroom.cpp
# Object files
OBJECTS = $(SOURCES:.cpp=.o)
# Executable name
EXECUTABLE = bathroom

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
