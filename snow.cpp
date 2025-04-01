#include "src/pali.hpp"
#include <string>
#include <memory>
#include <iostream>
#include <vector>
#include <random>


int main() {
    Engine engine(84, 84);
    std::string value = "o";
    int i = 0;
    while (i < 1000) {
    PixelProperties pp = PixelProperties(value, RGB(74, 74, 74));
    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(-100, 100);
    
    float x = distrib(gen);
    x += 200;
    x /= 1000;

    float r = distrib(gen);
    r += 340;
    r /= 84;

    int y = distrib(gen);
    y += 100;
    y /= 300;

    int v = distrib(gen);
    v += 100;
    v *= 3;
    auto po = std::make_unique<SnowObject>(SnowObject(Point(v, 84), pp, r, x, y));
    //po->setVelocity(Point(x / 24.0,y / 24.0));

    engine.addObject(std::move(po));
        std::cout << "Looping\n";
        engine.loop();
        i++;
    }
    return 0;
}