#include "src/pali.hpp"
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <vector>

#define HEIGHT 84
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
  while (1) {
    PixelProperties pp = PixelProperties(value, RGB(0, 0, 0), RGB(0, 0, 0));
    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(-100, 100);

    for (int i = 0; i < HEIGHT; i++) {   // y axis
      for (int j = 0; j < WIDTH; j++) { // x axis
        double zj = (j - (WIDTH / 2.0)) / (WIDTH / 4.0);
        double zi = (i - (HEIGHT / 2.0)) / (HEIGHT / 4.0);
        for (int v = 0; v < 32; v++) {
          zi = zj * zi + zi * zj;
          zj = zj * zj - zi * zi;
          if (zj * zj + zi * zi > 1000) {
            std::cout << "Diverged" << std::to_string(v) << std::endl;
            pp.setColor2(
                RGB((v << 3), 128 - (v << 3), 255 - (v << 3)));
            break;
          }
          if (v == 31) {
            pp.setColor2(RGB(0, 0, 0));
          }
        }
        std::cout << std::to_string(zi * zi + zj * zj) << std::endl;
        auto po = std::make_unique<PixelObject>(PixelObject(Point(j, i), pp));
        engine.addObject(std::move(po));
      }
    }
    // po->setVelocity(Point(x / 24.0,y / 24.0));

    std::cout << "Looping\n";
    engine.loop();
    while (1) {
    }
  }
  return 0;
}