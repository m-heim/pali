#ifndef PALI_HPP
#define PALI_HPP
#include <chrono>
#include <cmath>
#include <cstring>
#include "color.hpp"
#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <stack>
#include <string>
#include <sys/ioctl.h>
#include <thread>
#include <unistd.h>
#include <vector>

enum Screens {
  SCREEN = 100000,
  MENU = SCREEN + 1,
};

void enableRawMode();
void disableRawMode();
void i(std::queue<char> *q);

static uint64_t idgv = 0;

class Point {
public:
  Point() {}
  Point(float x, float y) {
    this->x = x;
    this->y = y;
  }
  float x;
  float y;
};

class PixelProperties {
public:
  PixelProperties() {}
  PixelProperties(std::string value, RGB color1, RGB color2) {
    this->value = value;
    this->color1 = color1;
    this->color2 = color2;
  }
  std::string value;
  RGB color1;
  RGB color2;
  std::string getValue() {
    // std::cout << "Getting value " + this->value + "\n";
    return "\u001b[48;2;" + std::to_string(this->color2.red) + ';' +
           std::to_string(this->color2.green) + ';' +
           std::to_string(this->color2.blue) + 'm' + "\u001b[38;2;" +
           std::to_string(this->color1.red) + ';' +
           std::to_string(this->color1.green) + ';' +
           std::to_string(this->color1.blue) + 'm' + this->value + "\u001b[0m";
  }
  void setValue(std::string value) { this->value = value; }
  void setColor1(RGB color) { this->color1 = color; }
  void setColor2(RGB color) { this->color2 = color; }
};

class Pixel {
public:
  Pixel() {}
  Pixel(Point p, PixelProperties pp) {
    this->p = p;
    this->pp = pp;
  }
  Point p;
  PixelProperties pp;
};

class Image {
public:
  Image() {}
  Image(int height, int width) {
    this->height = height;
    this->width = width;
    int i = height * width;
    std::cout << "Allocating " + std::to_string(i) + "\n";
    this->pixels = std::vector<PixelProperties>(i);
    this->PIXEL_EMPTY = PixelProperties(" ", RGB(255, 255, 255), RGB(0, 0, 0));
  }
  int height;
  int width;
  PixelProperties PIXEL_EMPTY;
  std::vector<PixelProperties> pixels;
  void print(Point p);
  void setPixel(PixelProperties pp, Point p) {
    this->pixels[(int)p.y * width + (int)p.x] = pp;
  }
  void setPixel(PixelProperties pp, float x, float y) {
    this->pixels[(((int)y) * width) + ((int)x)] = pp;
  }
  void clear() {
    for (int i = 0; i < this->height; i++) {
      for (int j = 0; j < this->width; j++) {
        this->setPixel(this->PIXEL_EMPTY, j, i);
      }
    }
  }
};

class EngineObject {
public:
  EngineObject() {}
  EngineObject(Point p, Point v) {
    this->p = p;
    this->v = v;
  }
  ~EngineObject() {}
  bool getVisible() { return this->visible; }
  void setVisible(bool visible) { this->visible = visible; }
  bool visible = true;
  Point p;
  uint64_t id;
  uint64_t getId() { return this->id; }
  Point v = Point(0, 0);
  void setVelocity(Point v) { this->v = v; }
  virtual void updateVelocity(uint64_t u) = 0;
  void updatePosition(uint64_t u) {
    this->p.x += this->v.x * u / 1000000.0;
    this->p.y += this->v.y * u / 1000000.0;
  }
  virtual std::vector<Pixel> getPixels() = 0;
  virtual void update(uint64_t u) = 0;
};

class PixelObject : public EngineObject {
public:
  PixelObject() {}
  PixelObject(Point p, PixelProperties pp) {
    this->p = p;
    this->pp = pp;
  }
  ~PixelObject() {}
  std::vector<Pixel> getPixels() override {
    std::vector<Pixel> v;
    if (this->visible) {
      v.push_back(Pixel(this->p, this->pp));
    }
    return v;
  }
  void updateVelocity(uint64_t u) override { this->updatePosition(u); }
  void update(uint64_t u) override { this->updateVelocity(u); }

private:
  PixelProperties pp;
};

class SphereObject : public EngineObject {
public:
  SphereObject() {}
  SphereObject(Point p, PixelProperties pp, float radius) {
    this->p = p;
    this->radius = radius;
    this->pp = pp;
  }
  void updateVelocity(uint64_t u) override { this->updatePosition(u); }
  void update(uint64_t u) override { this->updateVelocity(u); }

private:
  PixelProperties pp;
  float radius;
  std::vector<Pixel> getPixels() override {
    std::vector<Pixel> v;
    if (this->visible) {
      int eru = powf(this->radius, 2);
      for (int i = -radius; i <= this->radius; i++) {
        int pi = powf(i, 2);
        for (int j = -radius; j <= this->radius; j++) {
          int p = pi + powf(j, 2);
          if (p <= eru) {
            v.push_back(Pixel(Point(this->p.x + i, this->p.y + j), this->pp));
          }
        }
      }
    }
    return v;
  }
};

class RectangleObject : public EngineObject {
public:
  RectangleObject() {}
  RectangleObject(Point p, PixelProperties pp, int height, int width,
                  bool fill) {
    this->pp = pp;
    this->p = p;
    this->height = height;
    this->width = width;
    this->fill = fill;
  }
  std::vector<Pixel> getPixels() override {
    std::vector<Pixel> v;
    if (this->visible) {
      for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
          if (this->fill || i == 0 || i == (height - 1) || j == 0 ||
              j == (width - 1)) {
            v.push_back(Pixel(Point(this->p.x + j, this->p.y + i), this->pp));
          }
        }
      }
    }
    return v;
  }
  void updateVelocity(uint64_t u) override { this->updatePosition(u); }
  void update(uint64_t u) override { this->updateVelocity(u); }

private:
  int height;
  int width;
  PixelProperties pp;
  bool fill;
};

class SnowObject : public EngineObject {
public:
  SnowObject() {}
  SnowObject(Point p, PixelProperties pp, float radius, float velocity,
             float shift) {
    this->pp = pp;
    this->p = p;
    this->radius = radius;
    this->velocity = velocity;
    this->v = Point(0, -this->velocity);
    this->shift = shift;
    this->phase = 40;
  }
  void updateVelocity(uint64_t u) override {
    this->v = Point((phase - 40) * 0.01 * this->velocity, this->v.y);
    phase += 1;
    phase = phase % 80;
    updatePosition(u);
  }
  void update(uint64_t u) override { this->updateVelocity(u); }

private:
  PixelProperties pp;
  float shift;
  float velocity;
  float radius;
  int phase;
  std::vector<Pixel> getPixels() override {
    std::vector<Pixel> v;
    if (this->visible) {
      for (int i = -radius; i <= radius; i++) {
        v.push_back(Pixel(Point(this->p.x, this->p.y + i), this->pp));
        v.push_back(Pixel(Point(this->p.x + i, this->p.y), this->pp));
        v.push_back(Pixel(Point(this->p.x + i, this->p.y + i), this->pp));
        v.push_back(Pixel(Point(this->p.x + i, this->p.y - i), this->pp));
      }
    }
    return v;
  }
};

class StringObject : public EngineObject {
public:
  StringObject() {}
  StringObject(Point p, std::string s, RGB color1, RGB color2) {
    this->p = p;
    this->s = s;
    this->v = Point(0, 0);
    this->color1 = color1;
    this->color2 = color2;
  }

  void update(uint64_t u) override { this->updateVelocity(u); }
  void updateVelocity(uint64_t u) override { this->updatePosition(u); }
  void setString(std::string s) { this->s = s; }

private:
  std::string s;
  RGB color1;
  RGB color2;
  std::vector<Pixel> getPixels() override {
    std::vector<Pixel> vals;
    if (this->visible) {
      int i = 0;
      for (char v : this->s) {
        vals.push_back(Pixel(
            Point(this->p.x + i, this->p.y),
            PixelProperties(std::string(1, v), this->color1, this->color2)));
        i += 1;
      }
    }
    return vals;
  }
};

class InputFieldObject : public EngineObject {
public:
  InputFieldObject() {}
  InputFieldObject(Point p, std::string s, int height, int width, RGB color1,
                   RGB color2, bool border) {
    this->p = p;
    this->s = s;
    this->color1 = color1;
    this->color2 = color2;
    this->border = border;
    this->height = height;
    this->width = width;
  }
  std::string s;
  bool border;
  RGB color1;
  RGB color2;
  int height;
  int width;
  std::vector<Pixel> getPixels() override {
    std::vector<Pixel> vals;
    if (this->visible) {
      for (int i = 0; i < this->height; i += 1) {
        for (int j = 0; j < this->width; j += 1) {
          int posx = this->p.x + j;
          int posy = this->p.y + this->height - i;
          std::string vs = " ";
          if (i * this->width + j < this->s.length()) {
            vs = std::string(1, this->s.at(i * this->width + j));
          }
          vals.push_back(
              Pixel(Point(posx, posy),
                    PixelProperties(vs, this->color1, this->color2)));
        }
      }
    }
    return vals;
  }
  void updateVelocity(uint64_t u) override { this->updatePosition(u); }
  void update(uint64_t u) override { this->updateVelocity(u); }
};

class FrameObject : public EngineObject {
public:
  FrameObject() {}
  FrameObject(Point p, int width, int height, RGB color2, bool visible) {
    this->visible = visible;
    this->p = p;
    this->width = width;
    this->height = height;
    this->color2 = color2;
  }
  int id = 0;
  int width;
  int height;
  RGB color2;
  std::map<uint64_t, std::unique_ptr<EngineObject>> objects;
  uint64_t addObject(std::unique_ptr<EngineObject> eo);
  void removeObject(uint64_t id);
  EngineObject *getObject(uint64_t id);
  std::vector<Pixel> getPixels() override;
  void update(uint64_t u) override;
  void updateVelocity(uint64_t u) override;
};

class Engine {
public:
  Engine() {}
  Engine(int height, int width, bool verbose, double fps = 144.0) {
    this->verbose = verbose;
    this->image = Image(height, width);
    this->fps = fps;
    this->queue = std::queue<char>();
    this->j = std::thread(i, &this->queue);

    for (int i = Screens::SCREEN; i <= Screens::MENU; i++) {
      this->addScreen(i);
    }
    FrameObject *o =
        dynamic_cast<FrameObject *>(this->getObject(Screens::MENU));
    std::unique_ptr<StringObject> obj = std::make_unique<StringObject>(
        StringObject(Point(40, 20), "FPS 0 / " + std::to_string(this->fps),
                     RGB(255, 255, 255), RGB(0, 0, 0)));
    uint64_t v = o->addObject(std::move(obj));
    this->fpso = v;
    this->pushScreen(Screens::SCREEN);
  }
  ~Engine() { this->j.join(); }
  Image image;
  bool verbose; // be verbose
  int height_real;
  int width_real;
  double fps; // frames per second
  uint64_t fpso;
  std::queue<char> queue;       // input queue
  std::thread j;                // input thread
  Point position = Point(0, 0); // position on screen
  uint64_t id = 0;
  uint64_t u = 0; // microseconds for frame
  uint64_t p = std::chrono::duration_cast<std::chrono::microseconds>(
                   std::chrono::high_resolution_clock::now().time_since_epoch())
                   .count(); // previous position
  uint64_t pvg = 0;

  std::map<uint64_t, std::unique_ptr<EngineObject>> objects;
  std::stack<uint64_t> s;

  void setFpso();
  void updateMenu();
  void pushScreen(uint64_t id);
  void popScreen(uint64_t id);
  uint64_t getScreen();
  uint64_t addObject(std::unique_ptr<EngineObject> eo);
  char getInput(bool s);
  void addScreen(uint64_t id);
  void input();
  EngineObject *getObject(uint64_t id);
  void setPosition(Point p);
  void setRealPosition();
  void screenClear();
  Point getRealPosition();
  void removeObject(uint64_t id);
  void emptyObjs();
  void updateObjects();
  void loadObjects();
  void loop();
};

#endif
