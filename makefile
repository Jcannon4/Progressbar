CXX=g++
CXXFLAGS= -std=c++11 -g
LDFLAGS = -pthread

wordcount : 
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o wordcount a2.cpp

