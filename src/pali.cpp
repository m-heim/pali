#include "pali.hpp"
#include <cassert>
#include <chrono>
#include <iostream>
#include <queue>
#include <termios.h>
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
  char buf;
  while (1) {
    buf = std::cin.get();
    if (std::cin.eof()) {
      return;
    }
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
    for (int j = 0; j < this->width; j++) {
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
  s += "\u001b[" + std::to_string((int)p.y) + ';' + std::to_string((int)p.x) +
       'H';
  // usleep(10000);
  std::cout << s;
}

void Engine::loop() {
  auto valu = 1000000.0 / this->u; // some duration of prev frame
  auto si = std::make_unique<StringObject>(
      StringObject(Point(this->image.width - 10, 0), std::to_string(valu),
                   RGB(255, 255, 255), RGB(0, 0, 0)));
  // uint siv = this->addObject(std::move(si));
  this->input();
  this->updateMenu();
  this->image.clear();
  this->updateObjects();
  this->loadObjects();
  this->setRealPosition();
  Point p = this->getRealPosition();
  this->image.print(p);
  std::cout.flush();
  // this->removeObject(siv);

  uint64_t i1 =
      std::chrono::duration_cast<std::chrono::microseconds>(
          std::chrono::high_resolution_clock::now().time_since_epoch())
          .count();
  int64_t u = i1 - this->p;
  if (u < 0) {
    u = 0;
  }
  this->u = u;
  this->p = i1;
  auto i3 = (int)(1000000 / this->fps);
  if (u < i3) {
    usleep(i3 - u);
  }
}

uint64_t Engine::addObject(std::unique_ptr<EngineObject> eo) {
  uint64_t i_ = idgv;
  assert(this->objects.find(i_) == this->objects.end());
  this->objects[i_] = std::move(eo);
  idgv += 1;
  return i_;
}

void Engine::setFpso() {
  dynamic_cast<StringObject *>(
      dynamic_cast<FrameObject *>(this->getObject(Screens::MENU))
          ->getObject(fpso))
      ->setString("FPS " + std::to_string(1000000.0 / this->u) + " " + "/" +
                  " " + std::to_string(this->fps));
}

void Engine::updateMenu() { this->setFpso(); }

void Engine::pushScreen(uint64_t id) {
  if (!this->s.empty()) {
    this->getObject(this->s.top())->setVisible(false);
  }
  this->getObject(id)->setVisible(true);
  this->s.push(id);
}

void Engine::popScreen(uint64_t id) {
  this->getObject(id)->setVisible(false);
  this->s.pop();
  this->getObject(this->s.top())->setVisible(true);
}

uint64_t Engine::getScreen() { return this->s.top(); }

char Engine::getInput(bool s) {
  if (this->queue.empty()) {
    return 0;
  }
  char v = this->queue.front();
  if (s) {
    this->queue.pop();
  }
  return v;
}

void Engine::addScreen(uint64_t id) {
  this->objects[id] = std::make_unique<FrameObject>(
      FrameObject(Point(0, 0), this->image.width, this->image.height,
                  RGB(0, 0, 0), false));
}

void Engine::input() {
  char v = this->getInput(true);
  if (v == 'o') {
    if (this->getScreen() == Screens::MENU) {
      this->popScreen(Screens::MENU);
    } else {
      this->pushScreen(Screens::MENU);
    }
  }
}

EngineObject *Engine::getObject(uint64_t id) {
  if (this->objects.find(id) != this->objects.end()) {
    return this->objects[id].get();
  }
  return nullptr;
}

void Engine::setPosition(Point p) { this->position = p; }

void Engine::setRealPosition() {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  this->height_real = w.ws_row;
  this->width_real = w.ws_col;
}
void Engine::screenClear() { std::cout << "\u001b[2J" << std::endl; }

Point Engine::getRealPosition() {
  Point p;
  p.x = this->position.x;
  p.y = this->height_real - this->position.y;
  return p;
}

void Engine::removeObject(uint64_t id) {
  if (this->objects.find(id) != this->objects.end()) {
    this->objects.erase(id);
  }
}

void Engine::emptyObjs() { this->objects.clear(); }
void Engine::updateObjects() {
  for (auto &eo : this->objects) {
    eo.second->update(this->u);
  }
  //auto it = this->objects.begin();
  //while (it != this->objects.end()) {
  //  float x = it->second->p.x;
  //  float y = it->second->p.y;
  //  if (x < 0 || y < 0 || x >= this->image.width || y >= this->image.height) {
  //    this->removeObject(it->first);
  //    it = this->objects.begin();
  //
  //} else {
  //    it++;
  //}
  //}
}
void Engine::loadObjects() {
  for (auto &eo : this->objects) {
    for (Pixel &p : eo.second->getPixels()) {
      if (p.p.x < 0 || p.p.x > this->image.width || p.p.y < 0 ||
          p.p.y >= this->image.height) {
        continue;
      }
      this->image.setPixel(p.pp, p.p);
    }
  }
}
void loop();

uint64_t FrameObject::addObject(std::unique_ptr<EngineObject> eo) {
  uint64_t i_ = idgv;
  this->objects[i_] = std::move(eo);
  idgv += 1;
  return i_;
}
void FrameObject::removeObject(uint64_t id) {
  this->objects.erase(id);
}
EngineObject *FrameObject::getObject(uint64_t id) {
  auto eo = this->objects.find(id);
  if (eo != this->objects.end()) {
    return eo->second.get();
  }
  throw std::runtime_error("Obj");
}
std::vector<Pixel> FrameObject::getPixels() {
  std::vector<Pixel> v;
  if (this->visible) {
    for (auto &eo : this->objects) {
      auto pixels = eo.second->getPixels();
      for (auto &p : pixels) {
        Pixel pp = p;
        pp.p.x += this->p.x;
        pp.p.y += this->p.y;
        v.push_back(pp);
      }
    }
  }
  return v;
}
void FrameObject::update(uint64_t u) {
  for (auto &eo : this->objects) {
    eo.second->update(u);
  }
  this->updateVelocity(u);
  auto it = this->objects.begin();
  //while (it != this->objects.end()) {
    // std::cout << "Object" << "\n";
    //float x = it->second->p.x;
    //float y = it->second->p.y;
    // std::cout << "Position " + std::to_string(x) + " " + std::to_string(y)
    // + "\n";
    //if (x < 0 || y < 0 || x >= this->width || y >= this->height) {
      // std::cout << "Removing object" << std::endl;
    //  ;
    //  this->removeObject(it->first);
    //  it = this->objects.begin();
    //
    //} else {
    //  it++;
    //}
  //}
}
void FrameObject::updateVelocity(uint64_t u){ this->updatePosition(u); }