#include <iostream>
#include <memory>
#include <pali.hpp>
#include <random>
#include <string>

#define HEIGHT 84
#define WIDTH 84

int main() {
  Engine engine(84, 84, false, 43.0);
  std::string value = "o";
  int i = 0;
  while (i < 1000) {
    PixelProperties pp = PixelProperties(value, RGB(74, 74, 74), RGB(0, 0, 0));
    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(-10, 10);

    int x = distrib(gen);
    if (x == 0) {
      x = 1;
    }

    int y = distrib(gen);
    if (y == 0) {
      y = 1;
    }

    int r = distrib(gen);
    r += 30;
    r /= 7;

    auto po =
        std::make_unique<SphereObject>(SphereObject(Point(42, 42), pp, r));
    po->setVelocity(Point(x, y));

    engine.addObject(std::move(po));
    std::cout << "Looping\n";
    engine.loop();
    i++;
  }
  return 0;
}