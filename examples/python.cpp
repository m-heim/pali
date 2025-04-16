#include <cstdio>
#include <memory>
#include <pali.hpp>
#include <queue>
#include <random>
#include <string>
#include <termios.h>
#include <unistd.h>

#define HEIGHT 43
#define WIDTH 242

std::string getString(int v) {
  std::string s = "";
  if (v % 2 == 1) {
    s = "  ";
  } else {
    s = " ";
  }
  return s;
}

int main() {
s:
  Engine engine(HEIGHT, WIDTH, false, 24);
  auto screen = dynamic_cast<FrameObject *>(engine.getObject(Screens::SCREEN));
  std::queue<PixelObject> q;
  std::string value = "#";
  PixelProperties pp = PixelProperties(value, RGB(221, 0, 0), RGB(0, 0, 0));
  std::random_device rd;  // a seed source for the random number engine
  std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
  std::uniform_int_distribution<> distrib(0, WIDTH - 1);
  int x = 10;
  int y = 10;
  int dir = 0;
  std::size_t s = 0;
  std::queue<uint64_t> v;
  for (int i = 0; i < 10; i++) {
    pp.setValue(getString(dir));
    auto po = std::make_unique<PixelObject>(PixelObject(Point(x, y), pp));
    uint64_t vi = screen->addObject(std::move(po));
    v.push(vi);
    y += 1;
    s += 1;
  }
  int i = 0;
  while (1) {
    engine.loop();
    char vq = engine.getInput(false);
    if (vq == 'w') {
      if (dir != 2) {
        dir = 0;
      }
    } else if (vq == 'd') {
      if (dir != 3) {
        dir = 1;
      }
    } else if (vq == 's') {
      if (dir != 0) {
        dir = 2;
      }
    } else if (vq == 'a') {
      if (dir != 1) {
        dir = 3;
      }
    } else {
    }
    if (!v.empty()) {
      uint64_t vi = v.front();
      v.pop();
      screen->removeObject(vi);
    }
    if (x > 0 && x < WIDTH && y > 0 && y < HEIGHT) {
      std::string eo = "  ";
      auto so = std::make_unique<StringObject>(
          StringObject(Point(x, y), eo, RGB(0, 0, 0), RGB(221, 0, 0)));
      uint64_t vi = screen->addObject(std::move(so));
      v.push(vi);
    } else {
      engine.emptyObjs();
      auto so = std::make_unique<StringObject>(
          StringObject(Point(WIDTH / 2.0, HEIGHT / 2.0), "Game over",
                       RGB(0, 221, 0), RGB(221, 0, 0)));
      screen->addObject(std::move(so));
      engine.loop();
      int i = 0;
      while (engine.getInput(true) != 's') {
      }
    }
    if (i == 100) {
      int vx = distrib(gen);
      int vy = ((double)distrib(gen) / (double)WIDTH) * HEIGHT;
      auto ppv = pp;
      ppv.setValue("🍏");
      ppv.setColor2(RGB(0, 0, 0));
      auto po = std::make_unique<PixelObject>(PixelObject(Point(vx, vy), ppv));
      screen->addObject(std::move(po));
      i = 0;
    }
    if (dir == 0) {
      y += 1;
    }
    if (dir == 1) {
      x += 2;
    }
    if (dir == 2) {
      y -= 1;
    }
    if (dir == 3) {
      x -= 2;
    }
    i += 1;
  }
  return 0;
}