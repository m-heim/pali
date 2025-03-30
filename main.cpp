#include "src/pali.hpp"
#include <string>
#include <iostream>


int main() {
    Engine engine(43, 43);
    std::string value = "A";
    while (1) {
    PixelProperties pp = PixelProperties(value, RGB(74, 74, 74));
    PixelObject *po = new PixelObject(Point(24, 24), pp);
    po->setVelocity(Point(1, 1));

    engine.addObject(po);
        std::cout << "Looping\n";
        engine.loop();
    }
    return 0;
}