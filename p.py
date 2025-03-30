import logging
import random
import pali.image
import math
import os
import pali.engine

def verify() -> None:
    logging.info('%s', __file__)
    s = os.get_terminal_size()
    width = 64
    height = 64
    engine = pali.engine.Engine(height, width, "hello world", debug=False)
    j = 0
    i1 = 0
    i2 = 0
    while 1 == 1:
        for i in range(1):
            x = math.sin(i1)
            y = math.cos(i2)
            obj = pali.engine.PixelObject(pali.image.PixelProperties("A"), height // 2  + x * 10 + (int(j % 7) == 1) * 4, width // 2 + y * 10 + (int(j % 7) == 1) * 4)
            #obj.set_velocity((random.random() - 0.5) * 2, (random.random() - 0.5) * 2)
            engine.add_object(obj)
            i1 += 0.01
            i2 += 0.01
        engine.loop()
        j += 1

if __name__ == '__main__':
    verify()