#include "pali.hpp"
#include <iostream>
#include <chrono>
#include <unistd.h>

std::string PixelProperties::getValue() {
    //std::cout << "Getting value " + this->value + "\n";
    return " \u001b[38;2;" + std::to_string(this->color.red) + ';' + std::to_string(this->color.green) + ';' + std::to_string(this->color.blue) + 'm' + this->value + " ";
}

void Image::print() {
    std::string s = "\u001b[2J\n\n\n\n\n\n\n\u001b[48;2;0;0;0m";
    s.reserve(this->height * this->width * 4);
    for (int i = this->height - 1; i >= 0; i--) {
        //s += std::to_string(i);
        for (int j = 0; j < this->width; j++) {
            //std::string v = (this->pixels + (i * height) + j)->getValue();
            //if (v == "   ") {
            //    std::cout << "found value\n";
            //}
            s += this->pixels[(i * width) + j].getValue();
        }
        s += "\n";
    }
    std::cout << s;
}

void Engine::loop() {
    auto s = std::chrono::high_resolution_clock::now();
    this->image.clear();
    auto f = std::chrono::high_resolution_clock::now();
    if (this->verbose) {
        std::cout << std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(f - s).count()) << std::endl;
    }
    this->updateObjects();
    f = std::chrono::high_resolution_clock::now();
    if (this->verbose) {
        std::cout << std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(f - s).count()) << std::endl;
    }
    this->loadObjects();
    f = std::chrono::high_resolution_clock::now();
    if (this->verbose) {
        std::cout << std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(f - s).count()) << std::endl;
    }
    //std::string v = "";
    //for (int i = 0; i < 43; i++) {
    //    v += '\n';
    //}
    //std::cout << v;
    //std::cout << "\u001b[2J" << std::endl;
    this->image.print();
    if (this->verbose) {
        std::cout << "Objects " + std::to_string(this->objects.size()) + "\n";
    }
    f = std::chrono::high_resolution_clock::now();
    std::cout.flush();
    uint64_t i = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    this->u = i - this->p;
    this->p = i;
    if (this->verbose) {
        std::cout << std::to_string(this->u) << std::endl;
    }
    usleep(1000000);
}

uint64_t Engine::addObject(std::unique_ptr<EngineObject> eo) {
    if (this->verbose) {
        std::cout << "Object" << std::endl;
    }
    uint64_t id = this->id;
    this->id += 1;
    eo->id = id;
    this->objects.push_back(std::move(eo));
    if (this->verbose) {
        std::cout << "Valid" << std::endl;
    }
    return id;
}