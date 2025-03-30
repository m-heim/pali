#include "pali.hpp"
#include <iostream>
#include <chrono>
#include <unistd.h>

std::string PixelProperties::getValue() {
    //std::cout << "Getting value " + this->value + "\n";
    return " " + this->value + " ";
}

void Image::print() {
    std::string s = "";
    for (int i = 0; i < this->height; i++) {
        //s += std::to_string(i);
        for (int j = 0; j < this->width; j++) {
            std::string v = (this->pixels + (i * height) + j)->getValue();
            //if (v == "   ") {
            //    std::cout << "found value\n";
            //}
            s += (this->pixels + (i * width) + j)->getValue();
        }
        s += "\n";
    }
    std::cout << s;
}

void Image::setPixel(Point p, PixelProperties pp) {
    //std::cout << "Setting pixel " + std::to_string(p.x) + " " + std::to_string(p.y) + " value " + pp.value + "\n";
    *(this->pixels + (this->width * (int)p.y) + (int)p.x) = pp;
}

void Engine::loop() {
    auto s = std::chrono::high_resolution_clock::now();
    this->image.clear();
    this->updateObjects();
    this->loadObjects();
    std::string v = "";
    for (int i = 0; i < 43; i++) {
        v += '\n';
    }
    std::cout << v;
    this->image.print();
    std::cout << "Objects " + std::to_string(this->objects.size()) + "\n";
    auto f = std::chrono::high_resolution_clock::now();
    std::cout << std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(f - s).count()) << std::endl;
    usleep(10000);
}

void Engine::addObject(EngineObject *eo) {
    this->objects.push_back(eo);
}