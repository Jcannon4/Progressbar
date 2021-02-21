CXX=gcc

CXXFLAGS= -std=c99 -g

myls :  

	$(CXX) $(CXXFLAGS) -o myls a1.c 
