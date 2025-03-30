import logging
import pygments
from typing import Optional

import pygments.console

class RGB:
    def __init__(self, r: int, g: int, b: int):
        self._r = r
        self._g = g
        self._b = b

class PixelProperties:
    def __init__(self, char: str = " ", color: Optional[RGB] = RGB(255, 255, 255), bold: bool = False, cursive: bool = False):
        assert isinstance(char, str)
        self._char = char
        self._bold = bold
        self._color = color
        self._cursive = cursive

    def __str__(self) -> str:
        return f'PixelProperties(char={self._char})'

    def get(self) -> str:
        assert isinstance(self._char, str)
        return pygments.console.colorize('green', f' {self._char} ')
    
    def set_char(self, value: str) -> None:
        assert isinstance(value, str)
        self._char = value

    def set_color(self, value: RGB) -> None:
        assert isinstance(value, RGB)
        self._color = value

NON_PIXEL = PixelProperties()

class Image:
    def __init__(self, height: int, width: int, window_name: str, debug: bool = False):
        self._debug = debug
        self._height = height
        self._width = width
        self._image: list[list[PixelProperties]] = list(map(lambda _: list(map(lambda _: PixelProperties(), range(width))), list(map(lambda _: [], range(height)))))
        assert len(self._image) == height
        assert len(self._image[0]) == width
        self._window_name = window_name
        if self._debug is True:
            print('%s initialized', self)

    def __str__(self) -> str:
        return f'Image(height={self._height}, width={self._width}, window_name={self._window_name})'
    
    def add_point(self, x: int, y: int, v: PixelProperties):
        self._image[x][y] = v

    def is_on_screen(self, x: int, y: int) -> bool:
        return x >= 0 and x < self._height and y >= 0 and y < self._width

    def print_out(self):
        v = "" # + '_' * self._width + '\n'
        #if self._debug:
        #    print(self)
        for i in range(self._height):
            for j in range(self._width):
                pixel = self._image[i][j].get()
                assert isinstance(pixel, str)
                v += (str(i) + ' ' if self._debug else '') + pixel
            v += '\n'
        print(v)

    def get_point(self, x: int, y: int) -> PixelProperties:
        return self._image[x][y]
    
    def clear(self):
        for i in range(len(self._image)):
            for j in range(len(self._image[i])):
                self._image[i][j] = NON_PIXEL