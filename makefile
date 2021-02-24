CXX=g++
CXXFLAGS= -std=c++11 -g
# LDFLAGS = -pthread

# SRC = wordcount.cpp
# OBJ = $(SRC:.cpp=.o)
# EXEC = wordcount

# make : $(EXEC)
# $(EXEC) : $(OBJ)
# 		$(CXX) $(LDFLAGS) -o $@ $(OBJ) $(LBLIBS)

wordcount :
			$(CXX) $(CXXFLAGS) -o wordcount a2.cpp

