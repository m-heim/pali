#include <memory>
#include <pali.hpp>
#include <string>
#include <unistd.h>

#define HEIGHT 43
#define WIDTH 131

int main(int argc, char **argv) {
  bool verbose = false;
  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      std::string s = std::string(argv[i]);
      if (s == "--verbose") {
        verbose = true;
      }
    }
  }
  Engine engine(HEIGHT, WIDTH, verbose, 43.0);
  std::string value = " ";
  PixelProperties pp = PixelProperties(value, RGB(0, 0, 0), RGB(0, 255, 0));
  FrameObject *frame =
      dynamic_cast<FrameObject *>(engine.getObject(Screens::SCREEN));
  auto so1 = std::make_unique<StringObject>(StringObject(
      Point(10, 10), "Hello world", RGB(0, 0, 0), RGB(255, 255, 255)));
  auto so2 = std::make_unique<RectangleObject>(
      RectangleObject(Point(9, 9), pp, 3, 13, false));
  auto so3 = std::make_unique<InputFieldObject>(
      InputFieldObject(Point(43, 43), "Hello world", 24, 24, RGB(242, 242, 242),
                       RGB(84, 84, 84), false));
  auto so4 = std::make_unique<FrameObject>(
      FrameObject(Point(10, 10), 10, 10, RGB(131, 131, 131), true));
  auto so5 = std::make_unique<StringObject>(StringObject(
      Point(4, 4), "Hello world", RGB(255, 255, 255), RGB(0, 0, 0)));
  auto so6 = std::make_unique<SnowObject>(SnowObject(
      Point(1, 1), PixelProperties("o", RGB(255, 255, 255), RGB(0, 0, 0)), 3, 0,
      1));
  auto so7 = std::make_unique<SphereObject>(SphereObject(Point(7, 7), pp, 3));
  frame->addObject(std::move(so1));
  frame->addObject(std::move(so2));
  frame->addObject(std::move(so3));
  so4->addObject(std::move(so5));
  so4->addObject(std::move(so6));
  so4->addObject(std::move(so7));

  so4->setVelocity(Point(1, 0));
  frame->addObject(std::move(so4));
  engine.setPosition(Point(24, 24));
  int i = 0;
  while (1) {
    engine.loop();
    usleep(434);
    i += 1;
  }
  return 0;
}