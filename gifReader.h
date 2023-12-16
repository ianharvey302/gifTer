//Ian Harvey
//Gif Reader header file

#ifndef GIFREADER_H
#define GIFREADER_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

struct GCE {
  GCE() {}

  void print() {
    cout << disposalMethod << " " << userInputFlag << " " << transparentColorFlag << " " << delayTime << " " << transparentColorIndex << endl;
  }

  uint disposalMethod, delayTime, transparentColorIndex;
  bool userInputFlag, transparentColorFlag;
};

class gifReader {
 public:
  gifReader(ifstream* file);
  ~gifReader();
  vector<string*> generateFrames();
  
 private:
  void skipBytes(int byteAmount);
  void killIfNotValue(uint value);
  void insertIntoIndexStream(string toInsert, vector<int>* indexStream);
  uint readLittleEndian(int byteAmount);
  uint readOneByteInt();
  char* littleEndianBytes;
  GCE* graphicControlExtension;
  ifstream* file;
  
};

#endif
