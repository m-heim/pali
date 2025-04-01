#include "src/pali.hpp"
#include <string>
#include <memory>
#include <vector>
#include <cstdio>
#include <random>
#include <queue>
#include <iostream>


int main() {
    Engine engine(42, 42);
    std::queue<PixelObject> q;
    std::string value = "A";
    PixelProperties pp = PixelProperties(value, RGB(74, 74, 74));
    int x = 10;
    int y = 10;
    int dir = 1;

    for (int i = 0; i < 10; i++) {
        auto po = std::make_shared<PixelObject>(PixelObject(Point(x, y), pp));
        po->setVelocity(Point(0, 1));
        engine.addObject(std::move(po));
        y += 1;
    }
    int i = 0;
    while (1) {
        std::cout << "Looping\n";
        engine.loop();
        if (!std::cin.eof()) {
            char v = std::cin.get();
            if (v == 'd') {
                dir = 2;
            }
        }
        if (i == 100) {
            auto po = std::make_shared<PixelObject>(PixelObject(Point(x, y), pp));
            po->setVelocity(Point(0, 1));
            engine.addObject(std::move(po));
        }
        if (dir == 1) {
            y += 1;
        }
        if (dir == 2) {
            x += 1;
        }
        i += 1;
    }
    return 0;
}