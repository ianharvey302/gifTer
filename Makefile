#Ian Harvey
#Ascii Animate Makefile

CXX = clang++ $(CXXFLAGS)
CXXFLAGS = -O2 -Wall -fsanitize=address
OBJECTS = asciiAnimate.o animation.o gifReader.o

program: $(OBJECTS)
	$(CXX) $(OBJECTS) -o asciiAnimate

asciiAnimate.o: asciiAnimate.cpp

animation.o: animation.cpp animation.h

gifReader.o: gifReader.cpp gifReader.h color.h

.PHONY: clean
clean:
	-rm -f *.o *~ asciiAnimate
