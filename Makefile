
CXX=g++
CXXFLAGS=-g -std=c++11 -Wall -pedantic -I .

all: test-latency

test-latency: Makefile test-latency.cpp
	$(CXX) $(CXXFLAGS) -o test-latency test-latency.cpp