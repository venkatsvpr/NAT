# the compiler: gcc for C program, define as g++ for C++
CC = gcc

# Compiler flags:
CFLAGS  = -g -Wall

# variables
RM = rm -rf
TARGET = nat
HASHTABLE = hashtable
SRCPATH = src
BINPATH = bin

all: bin/$(TARGET)

$(BINPATH)/$(TARGET): $(SRCPATH)/$(TARGET).c $(SRCPATH)/$(HASHTABLE).c
	$(CC) $(CFLAGS) -o $(BINPATH)/$(TARGET) $(SRCPATH)/$(TARGET).c $(SRCPATH)/$(HASHTABLE).c

clean:
	$(RM) $(BINPATH)/$(TARGET)
