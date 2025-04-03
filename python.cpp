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
    Engine engine(42, 42, true);
    std::queue<PixelObject> q;
    std::string value = "A";
    PixelProperties pp = PixelProperties(value, RGB(74, 74, 74));
    int x = 10;
    int y = 10;
    int dir = 0;
    std::thread j(i, &queue);

    std::queue<std::shared_ptr<PixelObject>> v;
    for (int i = 0; i < 10; i++) {
        auto po = std::make_shared<PixelObject>(PixelObject(Point(x, y), pp));
        //po->setVelocity(Point(0, 1));
        engine.addObject(po);
        v.push(po);
        y += 1;
    }
    int i = 0;
    while (1) {
        std::cout << "Looping\n";
        engine.loop();
        if (!queue.empty()) {
            char v = queue.front();
            queue.pop();
            if (v == 'd') {
                dir = 2;
            }
        }
        //if (i == 100) {
            auto po = std::make_shared<PixelObject>(PixelObject(Point(x, y), pp));
            //po->setVelocity(Point(0, 1));
        //delete &o;
        
            engine.addObject(po);
            v.push(po);
            engine.removeObject(v.front());
            v.pop();
        //}
        if (dir == 0) {
            y += 1;
        }
        if (dir == 1) {
            x += 1;
        }
        if (dir == 2) {
            y -= 1;
        }
        i += 1;
        usleep(100000);
    }
    j.join();
    return 0;
}