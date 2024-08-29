CXX = g++
CXXFLAGS = -std=c++20 -g
LDFLAGS = -lstdc++ 
SDL2=/Library/Frameworks/SDL2.framework/Headers

INCLUDES=  -I$(SDL2) 
FRAMEWORKS= -F/Library/Frameworks -framework SDL2

all: 

	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(INCLUDES) $(FRAMEWORKS) RBB.cpp -o RBB

clean:
	rm -f RBB
	rm -f *.o
