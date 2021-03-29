# the compiler to be used
CC = g++

# compiler flags
CFLAGS = --std=c++11 -Wall -Werror -O3

# the build target executable
TARGET = loop_rooms

all: $(TARGET)
	
$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cpp

clean:
	$(RM) $(TARGET)

run:
	./$(TARGET) data.txt

