#include "src/pali.hpp"
#include <string>
#include <memory>
#include <vector>
#include <cstdio>
#include <random>
#include <queue>
#include <iostream>
#include <unistd.h>
#include <thread>
#include <termios.h>

void enableRawMode() {
    struct termios t;
    tcgetattr(STDIN_FILENO, &t); // Get current terminal attributes
    t.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &t); // Apply changes immediately
}

void disableRawMode() {
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag |= (ICANON | ECHO); // Re-enable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void i(std::queue<char> *q) {
    enableRawMode();
    std::cout << "Enter" << std::endl;

    char buf;

    while (1) {
        buf = std::cin.get();
        //std::cout << "Vals" << std::endl;
        //std::cout << buf << std::endl;
        if (std::cin.eof()) {
            //std::cout << "Eof" << std::endl;
            return;
        }
        //std::cout << "Have" << std::endl;
        q->push(buf);
    }
    disableRawMode();

}


int main() {
    std::queue<char> queue;
    Engine engine(42, 42, false);
    std::queue<PixelObject> q;
    std::string value = "A";
    PixelProperties pp = PixelProperties(value, RGB(74, 74, 74));
    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(0, 42);
    int x = 10;
    int y = 10;
    int dir = 0;
    std::thread j(i, &queue);
    std::size_t s = 0;
    std::queue<uint64_t> v;
    for (int i = 0; i < 10; i++) {
        auto po = std::make_unique<PixelObject>(PixelObject(Point(x, y), pp));
        //po->setVelocity(Point(0, 1));
        uint64_t vi = engine.addObject(std::move(po));
        v.push(vi);
        y += 1;
        s += 1;
    }
    int i = 0;
    while (1) {
        std::cout << "Looping\n";
        std::cout << "Queue" << std::to_string(v.size()) << std::endl;
        std::cout << std::to_string(x) << " " << std::to_string(y) << std::endl;
        engine.loop();
        if (!queue.empty()) {
            char v = queue.front();
            queue.pop();
            if (v == 'w') {
                if (dir != 2) {
                    dir = 0;
                }
            } else if (v == 'd') {
                if (dir != 3) {
                    dir = 1;
                }
            } else if (v == 's') {
                if (dir != 0) {
                    dir = 2;
                }
            } else if (v == 'a') {
                if (dir != 1) {
                    dir = 3;
                }
            }
        }
        if (!v.empty()) {
            uint64_t vi = v.front();
            v.pop();
            engine.removeObject(vi);
        }
        if (x > 0 && x < 42 && y > 0 && y < 42) {
            auto po = std::make_unique<PixelObject>(PixelObject(Point(x, y), pp));
            uint64_t vi = engine.addObject(std::move(po));
            v.push(vi);
        }
        if (i == 100) {
            int vx = distrib(gen);
            int vy = distrib(gen);
            auto ppv = pp;
            ppv.setValue("B");
            auto po = std::make_unique<PixelObject>(PixelObject(Point(vx, vy), ppv));
            engine.addObject(std::move(po));
            i = 0;
        }
        if (dir == 0) {
            y += 1;
        }
        if (dir == 1) {
            x += 1;
        }
        if (dir == 2) {
            y -= 1;
        }
        if (dir == 3) {
            x -= 1;
        }
        i += 1;
        usleep(100000);
    }
    j.join();
    return 0;
}