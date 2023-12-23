//Ian Harvey
//Gif Reader C++ file

#include "gifReader.h"
#include "color.h"



// Constructor initiating file variable
gifReader::gifReader(std::ifstream* file) : file(file) {
  littleEndianBytes = new char[4];
  graphicControlExtension = new GCE();
}

// Destructor deletes little Endian bytes
gifReader::~gifReader() {
  delete[] littleEndianBytes;
  delete graphicControlExtension;
}

// Get Methods
std::vector<uint> gifReader::getFrameDurations() {
  return frameDurations;
}

uint gifReader::getCanvasHeight() {
  return canvasHeight;
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
    std::cout << "Error reading '.gif' file. Unexpeced Value: " << (check[0] & 0xFF) << "\tShould Be: " << value << std::endl;
    delete this;
    exit(5);
  }
}

// Takes the index string and puts it in the indexStream
void gifReader::insertIntoIndexStream(std::string toInsert, std::vector<int>* indexStream) {
  std::string nextInt;
  for(int i = 0; i < toInsert.size(); i++) {
    // 48 is ASCII Zero
    if(toInsert[i] == ' ') {
      indexStream->push_back(stoi(nextInt));
      nextInt = "";
    }
    else {
      nextInt += toInsert[i];
    }
  }
  indexStream->push_back(stoi(nextInt));
}

// Converts bytes in the byte array into actual array
uint gifReader::readLittleEndian(int byteAmount) {
  int toReturn = 0;
  for(int i = 0; i < byteAmount; i++) {
    file->read(littleEndianBytes + i, 1);
    toReturn += (littleEndianBytes[i] & 0xFF) << (i * 4);
  }
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
    std::cout << toPrint[i];
  }
  std::cout << std::endl;
}

// Does the heavy lifting here :)
std::vector<std::string*> gifReader::generateFrames(uint pixelsPerPixel) {
  // Header Block reading
  char gifSig[3];
  file->read(gifSig, sizeof gifSig);
  char version[3];
  file->read(version, sizeof version);

  // Logical Screen Descriptor Reading
  uint canvasWidth = readLittleEndian(2);
  canvasHeight = readLittleEndian(2);
  pixelsPerPixel = std::min(std::min(canvasWidth,canvasHeight),pixelsPerPixel);
  uint LSDPacked = readOneByteInt();
  bool globalColorTableFlag = LSDPacked >> 7;
  uint globalColorTableSize = 2 << ((LSDPacked & 0x7));
  uint backgroundColorIndex = readOneByteInt();
  skipBytes(1);

  // Global Color Table
  std::vector<color> GCT(globalColorTableSize);
  if(globalColorTableFlag) {
    for(int i = 0; i < globalColorTableSize; i++) {
      char colorBytes[3];
      file->read(colorBytes, sizeof colorBytes);
      GCT[i].r = colorBytes[0] & 0xFF;
      GCT[i].g = colorBytes[1] & 0xFF;
      GCT[i].b = colorBytes[2] & 0xFF;
    }
  }

  // We begin the looping here
  std::vector<std::string*> frames;
  int pixels[canvasHeight][canvasWidth];
  for(int i = 0; i < canvasHeight; i++) {
    for(int j = 0; j < canvasWidth; j++) {
      pixels[i][j] = backgroundColorIndex;
    }
  }
  while((file->peek() & 0xFF) != 0x3B) {
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
	  frameDurations.push_back(graphicControlExtension->delayTime);
	  graphicControlExtension->transparentColorIndex = readOneByteInt();
	  killIfNotValue(0x00);
	  switch(graphicControlExtension->disposalMethod) {
	    case 1: {
	      // DoNothing
	      break;
	    }
	    case 2: {
	      for(int i = 0; i < canvasHeight; i++) {
		for(int j = 0; j < canvasWidth; j++) {
		  pixels[i][j] = backgroundColorIndex;
		}
	      }
	      break;
	    }
	    case 3: {
	      std::cout << "Error: This image uses a disposal method of 3 which is not yet supported by this program" << std::endl;
	      exit(100);
	      break;
	    }
	    default: {
	      std::cout << "Error: Unknown disposal method" << std::endl;
	      exit(100);
	      break;
	    }
	    
	  }
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
	  std::cout << "Unknown extension table label found in '.gif': " << extensionLabel << std::endl;
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
    // Variable is unused so commented out and bytes skipped for now...
    //uint imageHeight = readLittleEndian(2);
    skipBytes(2);
    uint IMGPacked = readOneByteInt();
    bool localColorTableFlag = IMGPacked >> 7;
    bool interlaceFlag = (IMGPacked >> 6) & 0x1;
    if(interlaceFlag) {
      std::cout << "Error: This image has an interlace flag and I am too lazy to figure out how it works :/" << std::endl;
      exit(100);
    }
    uint localColorTableSize = 2 << ((IMGPacked & 0x7));

    std::vector<color> LCT(localColorTableSize);
    if(localColorTableFlag) {
      for(int i = 0; i < localColorTableSize; i++) {
	char colorBytes[3];
	file->read(colorBytes, sizeof colorBytes);
	LCT[i].r = colorBytes[0] & 0xFF;
	LCT[i].g = colorBytes[1] & 0xFF;
	LCT[i].b = colorBytes[2] & 0xFF;
      }
    }

    // Image Data
    uint LZW = readOneByteInt();
    uint currentCodeSize = LZW + 1;
    uint codeCountTracker = (2 << (LZW - 1));
    uint nextTarget = (2 << (currentCodeSize - 1)) - 1;
    uint mask = 0;
    for(int i = 0; i < currentCodeSize; i++) {
      mask = (mask << 1) + 1;
    }
    uint codeBuffer = 0;
    uint currentCode;
    uint currentOffset = 0;
    std::vector<int> indexStream;
    std::vector<std::string> codeTable(2 << (LZW - 1));
    for(int i = 0; i < codeTable.size(); i++) {
      codeTable[i] = std::to_string(i);
    }
    codeTable.push_back("Clear");
    codeTable.push_back("EOI");
    bool first = true;
    std::string code;
    std::string oldCode;
    while((file->peek() & 0xFF) != 0x00) {
      uint subBlockSize = readOneByteInt();
      for(int i = 0; i < subBlockSize; i++) {
	codeBuffer += readOneByteInt() << currentOffset;
	currentOffset += 8;
	if(first) {
	  while(currentOffset < currentCodeSize) {
	    i++;
	    codeBuffer += readOneByteInt() << currentOffset;
	    currentOffset += 8;
	  }
	  codeBuffer = codeBuffer >> currentCodeSize;
	  currentOffset -= currentCodeSize;
	  while(currentOffset < currentCodeSize) {
	    i++;
	    codeBuffer += readOneByteInt() << currentOffset;
	    currentOffset += 8;
	  }
	  currentOffset -= currentCodeSize;
	  currentCode = codeBuffer & mask;
	  codeCountTracker++;
	  codeBuffer = codeBuffer >> currentCodeSize;
	  code = codeTable[currentCode];
	  insertIntoIndexStream(code, &indexStream);
	  oldCode = code;
	  first = false;
	  continue;
	}
	while(currentOffset > currentCodeSize) {
	  currentOffset -= currentCodeSize;
	  currentCode = codeBuffer & mask;
	  codeCountTracker++;
	  codeBuffer = codeBuffer >> currentCodeSize;
	  if(currentCode < codeTable.size()) {
	    code = codeTable[currentCode];
	    if(code == "EOI")
	      break;
	    if(code == "Clear") {
	      // TODO
	      continue;
	    }
	    std::string K;
	    for(char c : code) {
	      if(c == ' ') {
		break;
	      }
	      K += c;
	    }
	    codeTable.push_back(oldCode + " " + K);
	  }
	  else {
	    std::string K;
	    for(char c : oldCode) {
	      if(c == ' ') {
		break;
	      }
	      K += c;
	    }
	    codeTable.push_back(oldCode + " " + K);
	    code = oldCode + " " + K;
	  }
	  insertIntoIndexStream(code, &indexStream);
	  oldCode = code;
	  if(codeCountTracker == nextTarget) {
	    currentCodeSize++;
	    mask = (mask << 1) + 1;
	    nextTarget = (2 << (currentCodeSize - 1)) - 1;
	  }
	}
	if(code == "EOI")
	  break;
      }
      if(code == "EOI")
	break;
    }

    for(int i = 0; i < indexStream.size(); i++) {
      uint x = imageTop + i / imageWidth;
      uint y = imageLeft + i % imageWidth;
      pixels[x][y] = indexStream[i];
    }

    std::string* currentFrame = new std::string();
    if(pixelsPerPixel != 1) {
      uint cutLeft = (canvasWidth - canvasWidth / pixelsPerPixel * pixelsPerPixel) / 2;
      uint cutRight = canvasWidth - (canvasWidth / pixelsPerPixel * pixelsPerPixel);
      cutRight = (cutRight % 2 == 0) ? cutRight / 2 : cutRight / 2 + 1;
      uint cutTop = (canvasHeight - canvasHeight / pixelsPerPixel * pixelsPerPixel) / 2;
      uint cutBot = canvasHeight - (canvasHeight / pixelsPerPixel * pixelsPerPixel);
      cutBot = (cutBot % 2 == 0) ? cutBot / 2 : cutBot / 2 + 1;
      for(int i = cutTop; i < canvasHeight - cutBot; i += pixelsPerPixel) {
	for(int j = cutLeft; j < canvasWidth - cutRight; j += pixelsPerPixel) {
	  std::unordered_map<int, int> frequencyChart;
	  uint maxFrequency = 0;
	  uint mostFrequentIndex = -1;
	  for(int ii = i; ii < i + pixelsPerPixel; ii++) {
	    for(int jj = j; jj < j + pixelsPerPixel; jj++) {
	      if(frequencyChart.find(pixels[ii][jj]) != frequencyChart.end())
		frequencyChart[pixels[ii][jj]]++;
	      else
		frequencyChart[pixels[ii][jj]] = 1;
	      uint frequency = frequencyChart[pixels[ii][jj]];
	      if(frequency > maxFrequency) {
		maxFrequency = frequency;
		mostFrequentIndex = pixels[ii][jj];
	      }
	    }
	  }
	  if(mostFrequentIndex == graphicControlExtension->transparentColorIndex && graphicControlExtension->transparentColorFlag) {
	    *currentFrame += "  ";
	  }
	  else {
	    *currentFrame += colorToANSI(&(GCT[mostFrequentIndex]));
	  }
	}
	*currentFrame += "\n";
      }
    }
    else {
      for(int i = 0; i < canvasHeight; i++) {
	for(int j = 0; j < canvasWidth; j++) {
	  if(pixels[i][j] == graphicControlExtension->transparentColorIndex) {
	    *currentFrame += "  ";
	  }
	  else {
	    *currentFrame += colorToANSI(&(GCT[pixels[i][j]]));
	  }
	}	
	*currentFrame += "\n";
      }
    }
    *currentFrame += "\033[0m";
    frames.push_back(currentFrame);
    
    skipBytes(1);
  }
  
  return frames;
}
