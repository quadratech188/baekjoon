import math

from typing import List, Tuple

def min_tuple(tuple1, tuple2):
    if tuple1[0] <= tuple2[0]:
        if tuple1[1] <= tuple2[1]:
            return tuple1
        else:
            return tuple2

    else:
        if tuple2[1] <= tuple1[1]:
            return tuple2
        else:
            return tuple1

class Node:
    def __init__(self, start: int, end: int, values: List[int]):
        self.start = start
        self.end = end
        if self.size() != 1:
            self.left = Node(start, self.center(), values[:self.center() - start])
            self.right = Node(self.center(), end, values[self.center() - start:])

        if self.size() == 1:
            self.value = (self.start, values[0])
        else:
            self.value = min_tuple(self.left.value, self.right.value)
    def query(self, start, end):
        if self.size() == 1:
            return self.value

        if start <= self.start and self.end - 1 <= end:
            return self.value

        min_ = (0, math.inf)
        if start < self.center():
            min_ = min_tuple(min_, self.left.query(start, end))
        
        if self.center() <= end:
            min_ = min_tuple(min_, self.right.query(start, end))

        return min_
                    
    def set_value(self, index, value):
        if self.size() == 1:
            self.value = (index, value)
            return

        if index < self.center():
            self.left.set_value(index, value)
        else:
            self.right.set_value(index, value)

        self.value = min_tuple(self.left.value, self.right.value)

    def center(self) -> int:
        return int((self.start + self.end) / 2)

    def size(self) -> int:
        return self.end - self.start

class TreeIterator:
    def __init__(self, node: Node, target: int):
        self.root = node
        self.node = None
        self.target = target

    def __iter__(self):
        return self

    def __next__(self):
        if self.node is None:
            self.node = self.root
            return self.node

        if self.node.size() == 1:
            raise StopIteration

        if self.target < self.node.center():
            self.node = self.node.left
        else:
            self.node = self.node.right
        return self.node
def main():
    n =  int(input())   
    values = [int(x) for x in input().split()]

    root = Node(0, len(values), values)

    m = int(input())

    for _ in range(m):
        a, b, c = tuple(map(int, input().split()))

        if a == 1:
            root.set_value(b - 1, c)
            values[b - 1] = c

        if a == 2:
            print(root.query(b - 1, c - 1)[0] + 1)

    #breakpoint()
main()
