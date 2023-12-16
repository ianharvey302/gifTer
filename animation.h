//Ian Harvey
//Animation header file

#ifndef ANIMATION_H
#define ANIMATION_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class animation {
 public:
  animation(vector<string*> frames);
  ~animation();
  int length();
  string getFrame(int frameNumber);
  
 private:
  vector<string*> frames;
  
};

#endif
