#include "src/pali.hpp"
#include <memory>
#include <unistd.h>
#include <string>
#include <iostream>

#define HEIGHT 84
#define WIDTH 242

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
  double px = -0.34;
  double py = 0.68;
  double vxy = 1.1;
  std::vector<uint64_t> v;
  while (1) {
    PixelProperties pp = PixelProperties(value, RGB(0, 0, 0), RGB(0, 0, 0));
    for (int i = 0; i < HEIGHT; i++) {   // y axis
      for (int j = 0; j < WIDTH; j++) { // x axis
        double x = px + (((j - (WIDTH / 2.0)) / (WIDTH / 2.0)) * vxy);
        double y = py + (((i - (HEIGHT / 2.0)) / (HEIGHT / 2.0)) * vxy);
        double zj = 0;
        double zi = 0;
        for (int v = 0; v < 256; v += 1) {
          double zj2 = zj;
          zj = zj * zj - zi * zi + x;
          zi = zj2 * zi + zj2 * zi + y;
          if (zj * zj + zi * zi > 4) {
            //std::cout << "Diverged" << std::to_string(v) << std::endl;
            pp.setColor2(
                RGB(v, (128 - v) % 256, 255 - v));
            break;
          }
          if (v == 255) {
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
    usleep(400000);
    engine.emptyObjs();
    
    vxy = vxy / 1.041;
  }
  return 0;
}