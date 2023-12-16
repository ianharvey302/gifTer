//Ian Harvey
//Animation C++ file

#include "animation.h"

#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Constructor that copies the pointer for each element of the vector
animation::animation(vector<string*> framesToCopy) {
  for(string* s : framesToCopy) {
    frames.push_back(s);
  }
}

// Deconstructor deletes all of the frames in the vector
animation::~animation() {
  for(int i = 0; i < frames.size(); i++) {
    delete frames[i];
  }
}
