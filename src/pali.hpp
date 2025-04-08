#ifndef PALI_HPP
#define PALI_HPP
#include <chrono>
#include <cmath>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

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

class RGB {
public:
  RGB() {}
  RGB(int red, int green, int blue) {
    this->green = green;
    this->blue = blue;
    this->red = red;
  }
  inline bool operator==(const RGB &rhs) const {
    return std::memcmp(this, &rhs, sizeof(RGB)) == 0;
  }
  inline bool operator!=(const RGB &rhs) const {
    return std::memcmp(this, &rhs, sizeof(RGB)) != 0;
  }
  int green;
  int red;
  int blue;
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
  void print();
  void setPixel(PixelProperties pp, Point p) {
    this->pixels[(int)p.y * width + (int)p.x] = pp;
  }
  void setPixel(PixelProperties pp, float x, float y) {
    this->pixels[(int)y * width + (int)x] = pp;
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
  bool getExists() { return this->exists; }
  void setExists(bool exists) { this->exists = exists; }
  bool exists = true;
  Point p;
  uint64_t id;
  uint64_t getId() { return this->id; }
  Point v = Point(0, 0);
  void setVelocity(Point v) {
    // std::cout << "Setting velocity of " + std::to_string(this->p.x) + " " +
    // std::to_string(this->p.y) + " " + "\n";
    this->v = v;
  }
  virtual void updateVelocity() = 0;
  virtual std::vector<Pixel> getPixels() = 0;
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
    return std::vector<Pixel>{Pixel(this->p, this->pp)};
  }
  void updateVelocity() override {}

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
  void updateVelocity() override {}

private:
  PixelProperties pp;
  float radius;
  std::vector<Pixel> getPixels() override {
    std::vector<Pixel> v;
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
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        if (this->fill || i == 0 || i == (height - 1) || j == 0 ||
            j == (width - 1)) {
          v.push_back(Pixel(Point(this->p.x + j, this->p.y + i), this->pp));
        }
      }
    }
    return v;
  }
  void updateVelocity() override {}

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
  void updateVelocity() override {
    this->v = Point((phase - 40) * 0.01 * this->velocity, this->v.y);
    phase += 1;
    phase = phase % 80;
  }

private:
  PixelProperties pp;
  float shift;
  float velocity;
  float radius;
  int phase;
  std::vector<Pixel> getPixels() override {
    std::vector<Pixel> v;
    for (int i = -radius; i <= radius; i++) {
      v.push_back(Pixel(Point(this->p.x, this->p.y + i), this->pp));
      v.push_back(Pixel(Point(this->p.x + i, this->p.y), this->pp));
      v.push_back(Pixel(Point(this->p.x + i, this->p.y + i), this->pp));
      v.push_back(Pixel(Point(this->p.x + i, this->p.y - i), this->pp));
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

private:
  std::string s;
  RGB color1;
  RGB color2;
  std::vector<Pixel> getPixels() override {
    std::vector<Pixel> vals;
    int i = 0;
    for (char v : this->s) {
      vals.push_back(Pixel(
          Point(this->p.x + i, this->p.y),
          PixelProperties(std::string(1, v), this->color1, this->color2)));
      i += 1;
    }
    return vals;
  }
  void updateVelocity() override {}
};

class Engine {
public:
  Engine() {}
  Engine(int height, int width, bool verbose) {
    this->verbose = verbose;
    this->image = Image(height, width);
  }
  Image image;
  bool verbose;
  int u;
  Point position = Point(0, 0);
  uint64_t id = 0;
  uint64_t p = std::chrono::duration_cast<std::chrono::microseconds>(
                   std::chrono::high_resolution_clock::now().time_since_epoch())
                   .count();

  std::vector<std::unique_ptr<EngineObject>> objects;
  uint64_t addObject(std::unique_ptr<EngineObject> eo);
  void setPosition(Point p) { this->position = p; }

  void removeObject(uint64_t id) {
    for (auto it = this->objects.begin(); it != this->objects.end(); it++) {
      if ((*it)->id == id) {
        this->objects.erase(it);
        return;
      }
    }
  }
  void emptyObjs() { this->objects.clear(); }
  void updateObjects() {
    if (this->verbose) {
      std::cout << "Updating objects\n";
    }
    for (auto i = this->objects.end() - 1; i >= this->objects.begin();) {
      // std::cout << "Object" << "\n";
      float x = (*i)->p.x;
      float y = (*i)->p.y;
      // std::cout << "Position " + std::to_string(x) + " " + std::to_string(y)
      // + "\n";
      if (x < 0 || y < 0 || x >= this->image.width || y >= this->image.height) {
        // std::cout << "Removing object" << std::endl;
        ;
        this->objects.erase(i);
      }
      i--;
    }
    for (auto &eo : this->objects) {
      eo->updateVelocity();
      eo->p.x += eo->v.x * this->u / 1000000.0;
      eo->p.y += eo->v.y * this->u / 1000000.0;
      ;
    }
  }
  void loadObjects() {
    if (this->verbose) {
      std::cout << "Loading objects\n";
    }
    for (auto &eo : this->objects) {
      for (Pixel &p : eo->getPixels()) {
        if (p.p.x < 0 || p.p.x > this->image.width || p.p.y < 0 ||
            p.p.y >= this->image.height) {
          continue;
        }
        this->image.setPixel(p.pp, p.p);
      }
    }
  }
  void loop();
};

#endif
