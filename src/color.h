//Ian Harvey
//Color header file

#ifndef COLOR_H
#define COLOR_H

#include "definitions.h"
#include <unordered_map>
#include <string>

struct color {
  uint r, g, b;
  
  std::string hexString() {
    char rChars[3];
    sprintf(&(rChars[0]), "%x", r >> 4);
    sprintf(&(rChars[1]), "%x", r & 0xF);
    char gChars[3];
    sprintf(&(gChars[0]), "%x", g >> 4);
    sprintf(&(gChars[1]), "%x", g & 0xF);
    std::string rStr = (std::string) rChars;
    char bChars[3];
    sprintf(&(bChars[0]), "%x", b >> 4);
    sprintf(&(bChars[1]), "%x", b & 0xF);
    
    
    return (std::string) rChars + (std::string) gChars + (std::string) bChars;
  }

  color operator-(color other) {
    uint rMin = (r > other.r) ? (r - other.r) : (other.r - r);
    uint gMin = (g > other.g) ? (g - other.g) : (other.g - g);
    uint bMin = (b > other.b) ? (b - other.b) : (other.b - b);
    
    return { rMin, gMin, bMin };
  }
  
};


// I cannot believe that I manually set this
// There was definitely a better way to do this too but this code isn't that serious
extern std::unordered_map<std::string, std::tuple<char, color>> xtermANSI;

extern std::string colorToANSI(color* c);

#endif
