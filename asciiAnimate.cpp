#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include "animator.h"
#include "animation.h"
#include "gifReader.h"
using namespace std;


unordered_map<string, int> supportedExtensions = { {".gif", 1} };

int main(int argc, char **argv) {
  if(argc != 2) {
    cout << "Invalid argument amount" << endl;
    exit(1);
  }
  
  string fileName = argv[1];
  int extensionDelimiter = fileName.find_last_of(".");
  if(extensionDelimiter == -1) {
    cout << "Invalid file name" << endl;
    exit(2);
  }
  
  string fileExtension = fileName.substr(extensionDelimiter);
  if(!supportedExtensions[fileExtension]) {
    cout << "File extension not supported" << endl;
    exit(3);
  }
  
  ifstream toRead(fileName, ios::binary);
  if(!toRead.is_open()) {
    cout << "Error opening file" << endl;
    exit(4);
  }


  animation* anim = nullptr;
  switch(supportedExtensions[fileExtension]) {
    case 1: {
      //Gif
      gifReader* reader = new gifReader(&toRead);
      anim = new animation(reader->generateFrames());
      delete reader;
      break;
    }
      
    default: {
      cout << "Internal Error: Switch-Map missalignment" << endl;
      exit(5);
      break;
    }
  }
  delete anim;

  toRead.close();



  return 0;
}
