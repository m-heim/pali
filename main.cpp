#include "src/pali.hpp"
#include <string>
#include <memory>
#include <iostream>
#include <vector>
#include <random>


int main() {
    Engine engine(84, 84);
    std::string value = "A";
    while (1) {
    PixelProperties pp = PixelProperties(value, RGB(74, 74, 74));
    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(-4, 4);
    
    int x = distrib(gen);
    if (x == 0) {
        x = 1;
    }
    int y = distrib(gen);
    if (y == 0) {
        y = 1;
    }
    auto po = std::make_unique<PixelObject>(PixelObject(Point(42, 42), pp));
    po->setVelocity(Point(x,y));

    engine.addObject(std::move(po));
        std::cout << "Looping\n";
        engine.loop();
    }
    return 0;
}