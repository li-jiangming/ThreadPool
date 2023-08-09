.PHONY: all clean

CC=gcc
CXX=g++

TARGET=main

CFLAGS=-I.
CXXFLAGS=-I.
LDFLAGS=-lpthread

SRC=$(wildcard *.c *.cpp)
OBJ=$(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SRC)))

all:$(TARGET)

$(TARGET):$(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o:%.c
	$(CC) -c $< -o $@ $(CFLAGS)

%.o:%.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

clean:
	rm -f $(TARGET) $(OBJ)
