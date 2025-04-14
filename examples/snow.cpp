#include <pali.hpp>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <vector>

#define HEIGHT 43
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
  Engine engine(43, WIDTH, verbose);
  std::string value = "o";
  int i = 0;
  // auto s = std::make_unique<StringObject>(
  //     StringObject(Point(40, 20), "Hello world"));
  // engine.addObject(std::move(s));
  // std:: cout << "Loop" << std::endl;
  while (1) {
    PixelProperties pp = PixelProperties(value, RGB(74, 74, 74), RGB(0, 0, 0));
    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(-100, 100);

    float x = distrib(gen);
    x += 200;
    x /= 40;

    float r = distrib(gen);
    r += 240;
    r /= 43;

    int y = distrib(gen);
    y += 100;
    y /= 100;

    int v = distrib(gen);
    v += 100;
    v *= WIDTH * 1.0 / 100 / 2;

    if (i % 4 == 0) {
      auto po = std::make_unique<SnowObject>(
          SnowObject(Point(v, HEIGHT - 1), pp, r, x, y));
      // po->setVelocity(Point(x / 24.0,y / 24.0));

      engine.addObject(std::move(po));
    }
    engine.loop();
    usleep(100000);
    i++;
  }
  return 0;
}