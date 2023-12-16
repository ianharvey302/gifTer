//Ian Harvey
//Gif Reader C++ file

#include "gifReader.h"
#include "color.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <math.h>
using namespace std;


// Constructor initiating file variable
gifReader::gifReader(ifstream* file) : file(file) {
  littleEndianBytes = new char[4];
  graphicControlExtension = new GCE();
}

// Destructor deletes little Endian bytes
gifReader::~gifReader() {
  delete[] littleEndianBytes;
  delete graphicControlExtension;
}

// Jumps over unnecessary bytes
void gifReader::skipBytes(int byteAmount) {
  char useless[byteAmount];
  file->read(useless, byteAmount);
}

// Makes sure that we are looking in the right kind of file
void gifReader::killIfNotValue(uint value) {
  char check[1];
  file->read(check, 1);
  if((check[0] & 0xFF) != value) {
    cout << "Error reading '.gif' file. Unexpeced Value: " << (check[0] & 0xFF) << "\tShould Be: " << value << endl;
    delete this;
    exit(5);
  }
}

// Takes the index string and puts it in the indexStream
void gifReader::insertIntoIndexStream(string toInsert, vector<int>* indexStream) {
  if(toInsert == "Clear") {
    cout << toInsert << endl;
  }
  for(int i = 0; i < toInsert.size(); i++) {
    // 48 is ASCII Zero
    indexStream->push_back(toInsert[i] - 48);
  }
}

// Converts bytes in the byte array into actual array
uint gifReader::readLittleEndian(int byteAmount) {
  int toReturn = 0;
  for(int i = 0; i < byteAmount; i++) {
    file->read(littleEndianBytes + i, 1);
    toReturn += (littleEndianBytes[i] & 0xFF) << (i * 4);
  }
  //cout << toReturn << endl;
  littleEndianBytes[0] = 0;
  littleEndianBytes[1] = 0;
  littleEndianBytes[2] = 0;
  littleEndianBytes[3] = 0;
  return toReturn;
}

// Just reads one byte as an integer
uint gifReader::readOneByteInt() {
  char tmp[1];
  file->read(tmp, 1);
  return (tmp[0] & 0xFF);
}

void print(char* toPrint, int size) {
  for(int i = 0; i < size; i++) {
    cout << toPrint[i];
  }
  cout << endl;
}

// Does the heavy lifting here :)
vector<string*> gifReader::generateFrames() {
  // Header Block reading
  char gifSig[3];
  file->read(gifSig, sizeof gifSig);
  //print(gifSig, 3);
  char version[3];
  file->read(version, sizeof version);
  //print(version, 3);

  // Logical Screen Descriptor Reading
  uint canvasWidth = readLittleEndian(2);
  uint canvasHeight = readLittleEndian(2);
  uint LSDPacked = readOneByteInt();
  bool globalColorTableFlag = LSDPacked >> 7;
  uint globalColorTableResolution = pow(2, ((LSDPacked & 0x70) >> 4) + 1);
  uint globalColorTableSize = pow(2, (LSDPacked & 0x7) + 1);
  //printf("%x\n", (LSDPacked & 0xFF));
  //cout << globalColorTableFlag << " " << globalColorTableResolution << " " << globalColorTableSize << endl;
  uint backgroundColorIndex = readOneByteInt();
  //cout << backgroundColorIndex << endl;
  skipBytes(1);

  // Global Color Table
  vector<color> GCT(globalColorTableSize);
  if(globalColorTableFlag) {
    for(int i = 0; i < globalColorTableSize; i++) {
      char colorBytes[3];
      file->read(colorBytes, sizeof colorBytes);
      GCT[i].r = colorBytes[0] & 0xFF;
      GCT[i].g = colorBytes[1] & 0xFF;
      GCT[i].b = colorBytes[2] & 0xFF;
      cout << GCT[i].r << " " << GCT[i].g << " " << GCT[i].b << endl;
    }
  }

  // We begin the looping here
  vector<string*> toReturn;
  //while((file->peek() & 0xFF) != 0x3B) {

    while((file->peek() & 0xFF) == 0x21) {
      // Is an extension
      killIfNotValue(0x21);
      uint extensionLabel = readOneByteInt();
      switch(extensionLabel) {
        case 0xF9: {
	  // Graphic Control Extension
	  skipBytes(1);
	  uint GCEPacked = readOneByteInt();
	  graphicControlExtension->disposalMethod = (GCEPacked & 0x1C) >> 2;
	  graphicControlExtension->userInputFlag = GCEPacked & 0x2;
	  graphicControlExtension->transparentColorFlag = GCEPacked & 0x1;
	  graphicControlExtension->delayTime = readLittleEndian(2);
	  graphicControlExtension->transparentColorIndex = readOneByteInt();
	  killIfNotValue(0x00);
	  //graphicControlExtension->print();
	  break;
        }
        case 0xFF: {
	  skipBytes(17);
	  break;
        }
        case 0xFE:
        case 0x01: {
	  while((file->peek() & 0xFF) != 0x00) {
	    uint toSkip = readOneByteInt();
	    skipBytes(toSkip);
	  }
	  skipBytes(1);
	  break;
        }
        default: {
	  cout << "Unknown extension table label found in '.gif': " << extensionLabel << endl;
	  delete this;
	  exit(6);
        }
      }
    }

    // Image Descriptor
    killIfNotValue(0x2c);
    uint imageLeft = readLittleEndian(2);
    uint imageTop = readLittleEndian(2);
    uint imageWidth = readLittleEndian(2);
    uint imageHeight = readLittleEndian(2);
    uint IMGPacked = readOneByteInt();
    bool localColorTableFlag = IMGPacked >> 7;
    uint localColorTableSize = pow(2, (IMGPacked & 0x7) + 1);
    //cout << localColorTableFlag << " " << localColorTableSize << endl;

    vector<color> LCT(localColorTableSize);
    if(localColorTableFlag) {
      for(int i = 0; i < localColorTableSize; i++) {
	char colorBytes[3];
	file->read(colorBytes, sizeof colorBytes);
	LCT[i].r = colorBytes[0] & 0xFF;
	LCT[i].g = colorBytes[1] & 0xFF;
	LCT[i].b = colorBytes[2] & 0xFF;
	//cout << LCT[i].r << " " << LCT[i].g << " " << LCT[i].b << endl;
      }
    }
    
    // Preparing Canvas
    int pixels[canvasHeight][canvasWidth];
    for(int i = 0; i < canvasHeight; i++) {
      for(int j = 0; j < canvasWidth; j++) {
	pixels[i][j] = -1;
      }
    }
    
    // Image Data
    uint LZW = readOneByteInt();
    uint currentCodeSize = LZW + 1;
    uint nextTarget = pow(2, currentCodeSize) - 1;
    uint mask = 0;
    for(int i = 0; i < currentCodeSize; i++) {
      mask = (mask << 1) + 1;
    }
    //cout << "LZW: " << LZW << endl << endl;
    vector<int> codeStream;
    uint codeBuffer = 0;
    uint currentOffset = 0;
    uint codeCountTracker = pow(2, LZW) - 1;
    while((file->peek() & 0xFF) != 0x00) {
      uint subBlockSize = readOneByteInt();
      for(int i = 0; i < subBlockSize; i++) {
	codeBuffer += readOneByteInt() << currentOffset;
	currentOffset += 8;
	while(currentOffset > currentCodeSize) {
	  uint currentCode = codeBuffer & mask;
	  codeCountTracker++;
	  codeStream.push_back(currentCode);
	  codeBuffer = codeBuffer >> currentCodeSize;
	  currentOffset -= currentCodeSize;
	  if(codeCountTracker == nextTarget) {
	    currentCodeSize++;
	    mask = (mask << 1) + 1;
	    nextTarget = pow(2, currentCodeSize) - 1;
	  }
	}
      }
    }

    vector<int> indexStream;
    vector<string>* codeTable = new vector<string>(pow(2, LZW));
    for(int i = 0; i < codeTable->size(); i++) {
      (*codeTable)[i] = to_string(i);
    }
    codeTable->push_back("Clear");
    codeTable->push_back("EOI");
    insertIntoIndexStream((*codeTable)[codeStream[1]], &indexStream);
    for(int i = 2; i < codeStream.size(); i++) {
      int code = codeStream[i];
      int codeM1 = codeStream[i-1];
      if(code < codeTable->size()) {
        if((*codeTable)[code] == "EOI")
	  break;
	insertIntoIndexStream((*codeTable)[code], &indexStream);
	char K = (*codeTable)[code][0];
	//cout << (codeStream[i-1]) << endl;
	codeTable->push_back((*codeTable)[codeM1] + K);
      }
      else {
	char K = (*codeTable)[codeM1][0];
	insertIntoIndexStream((*codeTable)[codeM1] + K, &indexStream);
	codeTable->push_back((*codeTable)[codeM1] + K);
      }
    }
    cout << indexStream.size() << endl;
    cout << codeStream.size() << endl;
    delete codeTable;
    for(int i = imageTop; i < imageHeight; i++) {
      for(int j = imageLeft; j < imageWidth; j++) {
	
      }
    }
    skipBytes(1);
    //}
  
  vector<string*> nothing;
  return nothing;
}
