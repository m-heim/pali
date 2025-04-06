#include "src/pali.hpp"
#include <memory>
#include <unistd.h>
#include <string>
#include <iostream>

#define HEIGHT 43
#define WIDTH 84

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
  Engine engine(HEIGHT, WIDTH, verbose);
  std::string value = " ";
  double px = -1.41;
  double py = 0.111;
  double vxy = 3;
  std::vector<uint64_t> v;
  while (1) {
    PixelProperties pp = PixelProperties(value, RGB(0, 0, 0), RGB(0, 0, 0));
    for (int i = 0; i < HEIGHT; i++) {   // y axis
      for (int j = 0; j < WIDTH; j++) { // x axis
        double x = px + (((j - (WIDTH / 2.0)) / (WIDTH / 2.0)) * vxy);
        double y = py + (((i - (HEIGHT / 2.0)) / (HEIGHT / 2.0)) * vxy);
        double zj = x;
        double zi = y;
        for (int v = 0; v < 32; v++) {
          zj = zj * zj - zi * zi + x;
          zi = zj * zi + zi * zj + y;
          if (zj * zj + zi * zi > 4) {
            //std::cout << "Diverged" << std::to_string(v) << std::endl;
            pp.setColor2(
                RGB((v << 3), 128 - (v << 3), 255 - (v << 3)));
            break;
          }
          if (v == 31) {
            pp.setColor2(RGB(0, 0, 0));
          }
        }
        //std::cout << std::to_string(zi * zi + zj * zj) << std::endl;
        auto po = std::make_unique<PixelObject>(PixelObject(Point(j, i), pp));
        uint64_t ve = engine.addObject(std::move(po));
        v.push_back(ve);
      }
    }
    // po->setVelocity(Point(x / 24.0,y / 24.0));

    //std::cout << "Looping\n";
    engine.loop();
    //std::cout << 1 / vxy << std::endl;
    usleep(40000);
    engine.emptyObjs();
    
    vxy = vxy / 1.11;
  }
  return 0;
}