#ifndef PALI_HPP
#define PALI_HPP
#include <string>
#include <vector>
#include <iostream>
#include <memory>

class Point {
    public:
        Point(){}
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
        int green;
        int red;
        int blue;
};

class PixelProperties {
    public:
        PixelProperties(){}
        PixelProperties(std::string value, RGB color) {
            this->value = value;
            this->color = color;
        }
        std::string value;
        RGB color;
        std::string getValue();
};

class Image {
    public:
        Image(){}
        Image(int height, int width) {
            this->height = height;
            this->width = width;
            int i = sizeof(PixelProperties) * height * width;
            std::cout << "Allocating " + std::to_string(i) + "\n";
            this->pixels = (PixelProperties *) malloc(i);
            this->PIXEL_EMPTY = PixelProperties(" ", RGB(0, 0, 0));
        }
        int height;
        int width;
        PixelProperties PIXEL_EMPTY;
        PixelProperties *pixels;
        PixelProperties *e;
        void print();
        void setPixel(Point p, PixelProperties pp);
        void clear() {
            for (int i = 0; i < this->height; i++) {
                for (int j = 0; j < this->width; j++) {
                    this->setPixel(Point(j, i), this->PIXEL_EMPTY);
                }
            }
        }

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

class EngineObject {
    public:
        EngineObject(){}
        EngineObject(Point p, Point v) {
            this->p = p;
            this->v = v;
        }
        ~EngineObject(){}
        Point p;
        Point v;
        void setVelocity(Point v) {
            //std::cout << "Setting velocity of " + std::to_string(this->p.x) + " " + std::to_string(this->p.y) + " " + "\n";
            this->v = v;
        }
        virtual std::vector<Pixel> getPixels() = 0;
};

class PixelObject: public EngineObject {
    public:
        PixelObject(){}
        PixelObject(Point p, PixelProperties pp) {
            this->p = p;
            this->pp = pp;
        }
        ~PixelObject(){}
        PixelProperties pp;
        std::vector<Pixel> getPixels() override {
            return std::vector<Pixel> { Pixel(this->p, this->pp) };
        }
};

class Engine {
    public:
        Engine(){}
        Engine(int height, int width) {
            this->image = Image(height, width);
        }
        Image image;
        std::vector<std::shared_ptr<EngineObject>> objects;
        void addObject(std::shared_ptr<EngineObject> eo);
        void updateObjects() {
            std::cout << "Updating objects\n";
            for (auto eo : this->objects) {
                eo->p.x += eo->v.x;
                eo->p.y += eo->v.y;
            }
            for (std::vector<std::shared_ptr<EngineObject>>::iterator i = this->objects.end() - 1; i >= this->objects.begin();) {
                std::cout << "Object" << "\n";
                float x = (*i)->p.x;
                float y = (*i)->p.y;
                std::cout << "Position " + std::to_string(x) + " " + std::to_string(y) + "\n";
                if (x < 0 || y < 0 || x >= this->image.width || y >= this->image.height) {
                    this->objects.erase(i);
                }
                i--;
            }
        }
        void loadObjects() {
            std::cout << "Loading objects\n";
            for (auto eo : this->objects) {
                for (Pixel &p : eo->getPixels()) {
                    this->image.setPixel(p.p, p.pp);
                }
            }
        }
        void loop();
};


#endif
