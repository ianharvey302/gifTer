//Ian Harvey
//ASCII-Animate main C++ file

#include "animation.h"
#include "gifReader.h"
#include <unordered_map>
#include <iostream>
#include <string>


std::unordered_map<std::string, int> supportedExtensions = { {".gif", 1} };

int main(int argc, char **argv) {
  if(argc != 3) {
    std::cout << "Invalid argument amount" << std::endl;
    exit(1);
  }

  
  int pixelsPerPixel;
  try {
    pixelsPerPixel = std::stoi(argv[2]);
  }
  catch(std::invalid_argument e) {
    std::cout << "Argument 2 invalid input. Expected number, recieved: " << argv[2] << std::endl;
    exit(2);
  }
  if(pixelsPerPixel <= 0) {
    std::cout << "Argument 2 invalid input. Value must be greater than zero" << std::endl;
    exit(2);
  }
  
  
  std::string fileName = argv[1];
  int extensionDelimiter = fileName.find_last_of(".");
  if(extensionDelimiter == -1) {
    std::cout << "Invalid file name" << std::endl;
    exit(2);
  }
  
  std::string fileExtension = fileName.substr(extensionDelimiter);
  if(!supportedExtensions[fileExtension]) {
    std::cout << "File extension not supported" << std::endl;
    exit(3);
  }
  
  std::ifstream toRead(fileName, std::ios::binary);
  if(!toRead.is_open()) {
    std::cout << "Error opening file" << std::endl;
    exit(4);
  }


  animation* anim = nullptr;
  switch(supportedExtensions[fileExtension]) {
    case 1: {
      //Gif
      gifReader* reader = new gifReader(&toRead);
      anim = new animation(reader->generateFrames(pixelsPerPixel), reader->getFrameDurations(), reader->getCanvasHeight());
      delete reader;
      break;
    }
      
    default: {
      std::cout << "Internal Error: Switch-Map missalignment" << std::endl;
      exit(5);
      break;
    }
  }

  // Play the animation until interrupt
  anim->play();
  delete anim;

  // End Program
  toRead.close();
  return 0;
}
