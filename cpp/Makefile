.PHONY: all clean

CC=gcc
CXX=g++

TARGET=demo

CFLAGS=-I.
CXXFLAGS=-I. -std=c++11
LDFLAGS=-lpthread

SRC=$(wildcard *.c *.cpp)
OBJ=$(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SRC)))

all:$(TARGET)

$(TARGET):$(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o:%.c
	$(CC) -c $< -o $@ $(CFLAGS)

%.o:%.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

clean:
	rm -f $(TARGET) $(OBJ)
