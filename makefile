CXX=g++
CXXFLAGS= -std=c++11
LDFLAGS = -pthread

SRC = a2.cpp
OBJ = $(SRC:.cpp=.o)
EXEC = a2

make : $(EXEC)
$(EXEC) : $(OBJ)
		$(CXX) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)
