CXX := g++
CXXFLAGS := -std=c++11 -Wall -g  -DUSE_COLOR_TABLE

all: test

test: test.o PPM.o Maze.o
	$(CXX) $(CXXFLAGS) -o $@ $^

test.o: test.cpp Stack.h PPM.h
	$(CXX) $(CXXFLAGS) -c $<

PPM.o: PPM.cpp PPM.h
	$(CXX) $(CXXFLAGS) -c $<

Maze.o: Maze.cpp Maze.h PPM.h
	$(CXX) $(CXXFLAGS) -c $<