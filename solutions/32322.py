from typing import Tuple, Callable, Any

class SegmentNode:
    def __init__(self, segment: Tuple[int, int], combine_function: Callable[[Any, Any], Any], value_function: Callable[[int], Any]):
        self.segment = segment
        self.combine = combine_function
        self.value_function = value_function

        if self.size() == 1:
            self.value = value_function(segment[0])
            return

        self.left = SegmentNode( (segment[0], self.center()),
                                combine_function, value_function)

        self.right = SegmentNode( (self.center(), segment[1]),
                                 combine_function, value_function )

        self.value = self.combine(self.left.value, self.right.value)

    def size(self) -> int:
        return self.segment[1] - self.segment[0]

    def center(self) -> int:
        return int((self.segment[0] + self.segment[1]) / 2)

    def query(self, segment: Tuple[int, int]) -> Any:
        if segment[0] <= self.segment[0] and self.segment[1] <= segment[1]:
            return self.value
        
        if self.left.segment[1] <= segment[0]:
            return self.right.query(segment)

        if segment[1] <= self.right.segment[0]:
            return self.left.query(segment)

        return self.combine(self.left.query(segment),
                                  self.right.query(segment))

    def update(self, index):
        if self.size() == 1:
            self.value = self.value_function(index)
            return

        if index < self.center():
            self.left.update(index)
        else:
            self.right.update(index)

        self.value = self.combine(self.left.value, self.right.value)

n, t = tuple(map(int, input().split()))

values = [1 for _ in range(n)]

def value_function(index) -> int:
    return values[index]

def combine_function(val1, val2) -> int:
    return val1 + val2

root = SegmentNode((0, n), combine_function, value_function)

for _ in range(t):
    v = input().split()

    if v[0] == 'R':
        values[int(v[1]) - 1] = 0
        root.update(int(v[1]) - 1)
    
    else:
        result = root.query((int(v[1]) - 1, int(v[2])))
        print(result)
