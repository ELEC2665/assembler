# Set compiler
CC=g++

#Compile flags (-c to output object file .o)
CFLAGS=-c -Wall -std=c++11

all: asm.exe

asm.exe: main.o Parser.o Code.o
	$(CC) main.o Parser.o Code.o -o asm.exe

Parser.o: Parser.cpp
	$(CC) $(CFLAGS) Parser.cpp

Code.o: Code.cpp
	$(CC) $(CFLAGS) Code.cpp

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

clean:
	rm *.o asm.exe