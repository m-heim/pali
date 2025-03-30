from pali.image import Image
import pali.image
import time

class EngineObject:
    def __init__(self, x: float, y: float):
        self._debug = False
        self._x = x
        self._y = y
        self._x_velocity = 0
        self._y_velocity = 0
        self._t = 0

    def set_velocity(self, x_velocity: float, y_velocity: float):
        self._x_velocity = x_velocity
        self._y_velocity = y_velocity

    def set_position(self, x: float, y: float):
        self._x = x
        self._y = y

    def get_position(self) -> list:
        return [int(self._x), int(self._y)]

    def update_position(self):
        self._x += self._x_velocity
        self._y += self._y_velocity
        self._t += 1

    def get_points(self) -> list:
        raise NotImplementedError()

class PixelObject(EngineObject):
    def __init__(self, value: pali.image.PixelProperties, x: float, y: float):
        assert isinstance(value, pali.image.PixelProperties)
        super().__init__(x, y)
        self._value = value

    def get_points(self) -> list[list]:
        return [self.get_position() + [self._value]]
    
class PolygonObject(EngineObject):
    def __init__(self, points: list[list], x: float, y: float):
        super().__init__(x, y)
        self._points = points

    def get_points(self) -> list[list]:
        p = self.get_position()
        return list()
    
class LineObject(EngineObject):
    def __init__(self, value: pali.image.PixelProperties, p1: list[int], p2: list[int]):
        super().__init__(p1[0], p1[1])
        self._value = value
        self._p1 = p1
        self._p2 = p2
    def get_points(self) -> list[list]:
        v_x = self._p2[0] - self._p1[0]
        v_y = self._p2[1] - self._p1[1]
        points = []
        l = (v_x ** 2) + (v_y ** 2) ** 0.5
        i = 0
        while i < (l - 0.1):
            x = self._x + v_x * i / l
            y = self._y + v_y * i / l
            points.append([int(x), int(y), self._value])
            i += (min(1, l - i))
            #print(points)
        return points


class Engine:
    def __init__(self, height: int, width: int, window_name: str, debug: bool = False):
        self._debug = debug
        self._image = Image(height=height, width=width, window_name=window_name, debug=debug)
        self._objects: list[EngineObject] = [] # List of EngineObjects sorted by z index on screen

    def render(self) -> None:
        self._image.print_out()

    def update(self) -> None:
        objs = []
        for i, object in enumerate(self._objects):
            object.update_position()
            x, y = object.get_position()
            if self._image.is_on_screen(x, y) is False:
                objs.append(i)
                continue
            for point in object.get_points():
                if self._image.is_on_screen(point[0], point[1]) is True:
                    if self._image.get_point(point[0], point[1]) == pali.image.NON_PIXEL:
                        self._image.add_point(point[0], point[1], point[2])
        for i in reversed(objs):
            self._objects.pop(i)

    def add_object(self, object: EngineObject):
        self._objects.append(object)

    def loop(self):
        start = time.time()
        if self._debug is True:
            print(self._image)
        self._image.clear()
        self.update()
        for _ in range(1):
            print("\n" * 40)
        self.render()
        #print('Loop ' + str((time.time() - start) * 1000))
        #print('Objects ' + str(len(self._objects)))
        time.sleep(0.1)