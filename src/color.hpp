#ifndef _COLOR_HPP_
#define _COLOR_HPP_
#include <cstring>
#include <stdlib.h>
#include <unistd.h>

#define GREEN RGB(0, 255, 0)
#define WHITE RGB(255, 255, 255)
#define BLUE RGB(0, 0, 255)
#define RED RGB(255, 0, 0)
#define LIGHT_RED RGB(255, 131, 131)
#define LIGHT_BLUE RGB(131, 131, 255)
#define LIGHT_GREEN RGB(131, 255, 131)

class RGB {
public:
  RGB() {}
  RGB(int red, int green, int blue) {
    this->green = green;
    this->blue = blue;
    this->red = red;
  }
  inline bool operator==(const RGB &rhs) const {
    return std::memcmp(this, &rhs, sizeof(RGB)) == 0;
  }
  inline bool operator!=(const RGB &rhs) const {
    return std::memcmp(this, &rhs, sizeof(RGB)) != 0;
  }
  int green;
  int red;
  int blue;
};

#endif