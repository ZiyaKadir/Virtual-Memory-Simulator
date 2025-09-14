# Compiler
CC := g++

# Compiler flags
CFLAGS := -Wall -Wextra -Werror

# Source files
SRCS := $(wildcard *.cpp)

# Object files
OBJS := $(SRCS:.cpp=.o)

ARGS := 3 3 5 LRU 100 diskFileName.dat

# Target executable
TARGET := sortArrays

# Default target
all: run

run: compile
	./$(TARGET) $(ARGS)

compile: $(OBJS)
	g++  $(OBJS) -o $(TARGET) -pthread

# Clean up object files and the target executable
clean:
	rm -f $(OBJS) $(TARGET)
	rm -f diskFileName.dat
	rm -f diskFileName2.dat
	rm -f diskFileName2.txt

link: $(SRC)
	g++ -c main.cpp -o main.o -lpthread
	$(CC) -c $(CFLAGS) $(SRCS)

