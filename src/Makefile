#Ian Harvey
#Ascii Animate Makefile

CXX = clang++ $(CXXFLAGS)
CXXFLAGS = -O2 -Wall -fsanitize=address
OBJECTS = asciiAnimate.o animation.o gifReader.o color.o

program: $(OBJECTS)
	$(CXX) $(OBJECTS) -o ../asciiAnimate

asciiAnimate.o: asciiAnimate.cpp animation.h gifReader.h

color.o: color.cpp color.h definitions.h

animation.o: animation.cpp animation.h definitions.h

gifReader.o: gifReader.cpp gifReader.h color.h definitions.h


.PHONY: clean
clean:
	-rm -f *.o *~ asciiAnimate
