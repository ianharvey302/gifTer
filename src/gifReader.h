//Ian Harvey
//Gif Reader header file

#ifndef GIFREADER_H
#define GIFREADER_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

struct GCE {
  GCE() {}

  uint disposalMethod, delayTime, transparentColorIndex;
  bool userInputFlag, transparentColorFlag;
};

class gifReader {
 public:
  gifReader(std::ifstream* file);
  ~gifReader();
  std::vector<std::string*> generateFrames(uint pixelsPerPixel);
  std::vector<uint> getFrameDurations();
  uint getCanvasHeight();
  
 private:
  void skipBytes(int byteAmount);
  void killIfNotValue(uint value);
  void insertIntoIndexStream(std::string toInsert, std::vector<int>* indexStream);
  uint readLittleEndian(int byteAmount);
  uint readOneByteInt();
  std::vector<uint> frameDurations;
  uint canvasHeight;
  char* littleEndianBytes;
  GCE* graphicControlExtension;
  std::ifstream* file;
  
};

#endif
