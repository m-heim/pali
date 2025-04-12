#include "pali.hpp"
#include <memory>
#include <unistd.h>
#include <string>
#include <iostream>

#define HEIGHT 43
#define WIDTH 131

int main(int argc, char **argv) {
  bool verbose = false;
  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      std::string s = std::string(argv[i]);
      if (s == "--verbose") {
        verbose = true;
      }
    }
  }
  Engine engine(HEIGHT, WIDTH, verbose, 141.0);
  std::string value = " ";
  PixelProperties pp = PixelProperties(value, RGB(0, 0, 0), RGB(0, 255, 0));
  auto so1 = std::make_unique<StringObject>(StringObject(Point(10, 10), "Hello world", RGB(0, 0, 0), RGB(255, 255, 255)));
  engine.addObject(std::move(so1));
  auto so2 = std::make_unique<RectangleObject>(RectangleObject(Point(9, 9), pp, 3, 13, false));
  engine.addObject(std::move(so2));
  auto so3 = std::make_unique<InputFieldObject>(InputFieldObject(Point(43, 43), "Hello world2 example of a input fieldHello world2 example of a input fieldHello world2 example of a input fieldHello world2 example of a input fieldHello world2 example of a input fieldHello world2 example of a input field", 24, 24, RGB(242, 242, 242), RGB(84, 84, 84), false));
  engine.addObject(std::move(so3));
  engine.setPosition(Point(24, 24));
  int i = 0;
  while (1) {
    engine.loop();
    usleep(434);
    //engine.emptyObjs();
    i += 1;
  }
  return 0;
}