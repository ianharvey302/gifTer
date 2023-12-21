//Ian Harvey
//Animation C++ file

#include "animation.h"

// Constructor that copies the pointer for each element of the vector
animation::animation(std::vector<std::string*> framesToCopy, std::vector<uint> frameDurations, uint canvasHeight) {
  for(std::string* s : framesToCopy) {
    frames.push_back(s);
  }
  for(int i : frameDurations) {
    durations.push_back(i);
  }
  height = canvasHeight;
}

// Deconstructor deletes all of the frames in the vector
animation::~animation() {
  for(int i = 0; i < frames.size(); i++) {
    delete frames[i];
  }
}

void animation::play() {
  system("clear");
  bool kill = false;
  int frameNumber = 0;
  std::cin.sync_with_stdio(false);
  std::cin.clear();
  while(!kill) {
    std::cout << *(frames[frameNumber]) << std::endl;
    std::cout << "\033[1;31mPress (\u23CE) to terminate the animation\033[0m" << std::endl;
    usleep(durations[frameNumber] * 10000);
    frameNumber = (frameNumber + 1) % frames.size();
    system("clear");
    if (std::cin.rdbuf()->in_avail() > 0) {
      kill = true;
    }
  }
  std::cin.ignore(0xFFFFFFFF, '\n');
}
