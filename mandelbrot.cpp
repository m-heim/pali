#include "src/pali.hpp"
#include <string>
#include <memory>
#include <iostream>
#include <vector>
#include <random>


int main(int argc, char **argv) {
    bool verbose = false;
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            std::string s = std::string(argv[i]);
            if (s =="--verbose") {
                verbose = true;
            }
        }
    }
    Engine engine(84, 84, verbose);
    std::string value = "#";
    while (1) {
    PixelProperties pp = PixelProperties(value, RGB(74, 74, 74));
    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(-100, 100);

    for (int i = 0; i < 84; i++) { // y axis
        for (int j = 0; j < 84; j++) { // x axis
            double zj = (j  - 42) / 24.0;
            double zi = (i - 42) / 24.0;
            for (int v = 0; v < 8; v++) {
                zi = zj * zi + zi * zj;
                zj = zi * zi - zj * zj;
                if (zi * zi + zj * zj > 10000000) {
                    std::cout << "Diverged" << std::to_string(v) << std::endl;
                    pp.setColor(RGB((v >> 2) * 24, (v >> 1) & 0x03 * 24, (v & 0x03) * 24));
                    break;
                }
                if (v == 99) {
                    pp.setColor(RGB(0, 0, 0));
                }
            }
            std::cout << std::to_string(zi * zi + zj * zj) << std::endl;
            auto po = std::make_unique<PixelObject>(PixelObject(Point(j, i), pp));
            engine.addObject(std::move(po));
        }
    }
    //po->setVelocity(Point(x / 24.0,y / 24.0));

        std::cout << "Looping\n";
        engine.loop();
        while (1) {}
    }
    return 0;
}