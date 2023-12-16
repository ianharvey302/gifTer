#Ian Harvey
#Ascii Animate Makefile

CXX = clang++ $(CXXFLAGS)
CXXFLAGS = -O2 -fsanitize=address
OBJECTS = asciiAnimate.o animator.o animation.o gifReader.o

program: $(OBJECTS)
	$(CXX) $(OBJECTS) -o asciiAnimate
	make objectless

object: $(OBJECTS)
	$(CXX) $(OBJECTS) -o asciiAnimate

asciiAnimate.o: asciiAnimate.cpp

animator.o: animator.cpp animator.h

animation.o: animation.cpp animation.h

gifReader.o: gifReader.cpp gifReader.h color.h

.PHONY: clean
clean:
	-rm -f *.o *~ asciiAnimate

.PHONY: objectless
objectless:
	-rm -f *.o *~
