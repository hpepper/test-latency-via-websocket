
CXX=g++
CXXFLAGS=-g -std=c++11 -Wall -pedantic -I .

all: test-latency

test-latency: Makefile test-latency.cpp websocketpp/transport/bare/endpoint.hpp websocketpp/transport/bare/connection.hpp
	$(CXX) $(CXXFLAGS) -o test-latency test-latency.cpp