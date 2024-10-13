from typing import List, Tuple

class Bus:
    def __init__(self, x1, y1, x2, y2):
        self.x1 = x1
        self.y1 = y1
        self.x2 = x2
        self.y2 = y2
        self.connections: List[Bus] = []

        if x1 == x2:
            self.axis = 'y'
        else:
            self.axis = 'x'

    def __str__(self):
        return f"{self.x1} {self.y1} {self.x2} {self.y2}"

    def __repr__(self):
        return self.__str__()

    def in_range_x(self, x):
        return min(self.x1, self.x2) <= x <= max(self.x1, self.x2)

    def in_range_y(self, y):
        return min(self.y1, self.y2) <= y <= max(self.y1, self.y2)

    def contains(self, pos):
        if self.axis == 'y':
            return pos[0] == self.x1 and self.in_range_y(pos[1])
        else:
            return pos[1] == self.y1 and self.in_range_x(pos[0])


    def intersects(self, other: 'Bus'):
        if self.axis == other.axis:
            if self.axis == 'y':
                if self.x1 != other.x1:
                    return False

                return max(self.y1, self.y2) >= min(other.y1, other.y2) \
                        and max(other.y1, other.y2) >= min(self.y1, self.y2)
            else:
                if self.y1 != other.y1:
                    return False

                return max(self.x1, self.x2) >= min(other.x1, other.x2) \
                        and max(other.x1, other.x2) >= min(self.x1, self.x2)
        
        if self.axis == 'x':
            return self.in_range_x(other.x1) and other.in_range_y(self.y1)
        else:
            return self.in_range_y(other.y1) and other.in_range_x(self.x1)


def main():
    m, n = tuple(map(int, input().split()))
    k = int(input())
    buses: List[Bus] = [None] * k
    for _ in range(k):
        b, x1, y1, x2 ,y2 = tuple(map(int, input().split()))
        buses[b - 1] = Bus(x1, y1, x2, y2)

    sx, sy, dx, dy = tuple(map(int, input().split()))
    start = (sx, sy)
    end = (dx, dy)

    start_buses = set()
    end_buses = set()

    for bus in buses:
        for other in buses:
            if bus.intersects(other):
                bus.connections.append(other)
        
        if bus.contains(start):
            start_buses.add(bus)

        if bus.contains(end):
            end_buses.add(bus)

    queue = [
        {'bus': bus, 'times': 0} for bus in start_buses
    ]
    used_buses = start_buses.copy()

    while True:
        state = queue.pop(0)

        #print('Popped', state)

        #print(state['bus'].x1, state['bus'].y1)

        if state['bus'] in end_buses:
            print(state['times'] + 1)
            break
        
        for next_bus in state['bus'].connections:
            if next_bus in used_buses:
                continue
            used_buses.add(next_bus)
            queue.append(
                {'bus': next_bus, 'times': state['times'] + 1}
            )
            #print(queue)

        #print('sdf')

main()
