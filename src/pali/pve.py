import logging
import pali.image
from math import sin, cos

def init() -> None:
    logging.info('%s', __file__)
    image = pali.image.Image(32, 32, "hello world")
    for i in range(32):
        for j in range(32):
            #if i == j or (32 - i) == j or i == 16 or j == 16:
            image.get_point(i, j).set_char(chr(ord('0') + (i + j) % 10))
    image.print_out()

if __name__ == '__main__':
    init()