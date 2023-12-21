#Ian Harvey
#Root Makefile

CXX = clang++ $(CXXFLAGS)
CXXFLAGS = -O2 -Wall -fsanitize=address
OBJECTS = asciiAnimate.o animation.o gifReader.o

make:
	$(MAKE) -C src

.PHONY: clean
clean:
	-rm -f *.o *~ asciiAnimate
	$(MAKE) -C src clean
