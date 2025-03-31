#include "src/pali.hpp"
#include <string>
#include <memory>
#include <iostream>
#include <vector>
#include <random>
#include <queue>


int main() {
    Engine engine(42, 42);
    std::queue<PixelObject> q;
    std::string value = "A";
    PixelProperties pp = PixelProperties(value, RGB(74, 74, 74));

    auto po = std::make_shared<PixelObject>(PixelObject(Point(10, 10), pp));
    

    engine.addObject(std::move(po));
    int x = 0;
    int y = 0;
    while (1) {
        std::cout << "Looping\n";
        engine.loop();
    }
    return 0;
}