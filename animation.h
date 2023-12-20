//Ian Harvey
//Animation header file

#ifndef ANIMATION_H
#define ANIMATION_H

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

class animation {
 public:
  animation(std::vector<std::string*> framesToCopy, std::vector<uint> frameDurations, uint canvasHeight);
  ~animation();
  int length();
  std::string getFrame(int frameNumber);
  void play();
  
 private:
  std::vector<std::string*> frames;
  std::vector<int> durations;
  uint height;
  
};

#endif
