#include "pali.hpp"
#include <chrono>
#include <iostream>
#include <termios.h>
#include <queue>
#include <unistd.h>

void enableRawMode() {
  struct termios t;
  tcgetattr(STDIN_FILENO, &t);          // Get current terminal attributes
  t.c_lflag &= ~(ICANON | ECHO);        // Disable canonical mode and echo
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
    // std::cout << "Vals" << std::endl;
    // std::cout << buf << std::endl;
    if (std::cin.eof()) {
      // std::cout << "Eof" << std::endl;
      return;
    }
    // std::cout << "Have" << std::endl;
    q->push(buf);
  }
  disableRawMode();
}

void Image::print(Point p) {
  std::string s = "\u001b[2J\n";
  RGB color1 = RGB(0, 0, 0);
  RGB color2 = RGB(0, 0, 0);
  s.reserve(this->height * this->width * 4);
  for (int i = this->height - 1; i >= 0; i--) {
    // s += std::to_string(i);
    for (int j = 0; j < this->width; j++) {
      // std::string v = (this->pixels + (i * height) + j)->getValue();
      // if (v == "   ") {
      //     std::cout << "found value\n";
      // }
      auto p = this->pixels[(i * width) + j];
      if (j == 0 || p.color2 != color2) {
        s += "\u001b[48;2;" + std::to_string(p.color2.red) + ';' +
             std::to_string(p.color2.green) + ';' +
             std::to_string(p.color2.blue) + 'm';
      }
      if (j == 0 || p.color1 != color1) {
        s += "\u001b[38;2;" + std::to_string(p.color1.red) + ';' +
             std::to_string(p.color1.green) + ';' +
             std::to_string(p.color1.blue) + 'm';
      }
      color1 = p.color1;
      color2 = p.color2;
      s += p.value + "";
    }
    s += "\u001b[0m\n";
  }
  s+= "\u001b[" + std::to_string((int)p.y) + ';' + std::to_string((int)p.x) + 'H';
  //usleep(10000);
  std::cout << s;
}

void Engine::loop() {
  auto valu = 1000000.0 / this->u; // some duration of prev frame
  auto si = std::make_unique<StringObject>(StringObject(Point(this->image.width - 10, 0), std::to_string(valu), RGB(255, 255, 255), RGB(0, 0, 0)));
  uint siv = this->addObject(std::move(si));
  this->image.clear();
  this->updateObjects();
  this->loadObjects();
  this->setRealPosition();
  Point p = this->getRealPosition();
  this->image.print(p);
  std::cout.flush();
  this->removeObject(siv);

  uint64_t i1 = std::chrono::duration_cast<std::chrono::microseconds>(
                   std::chrono::high_resolution_clock::now().time_since_epoch())
                   .count();
  this->u = i1 - this->p;
  this->p = i1;
  auto i3 = (int) (1000000 / this->fps);
  if (u < i3) {
    usleep(i3 - u);
  }
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