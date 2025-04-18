from typing import List

class Node:
    def __init__(self, start: int, end: int, values: List[int]):
        self.start = start
        self.end = end
        if self.size() != 1:
            self.left = Node(start, self.center(), values[:self.center() - start])
            self.right = Node(self.center(), end, values[self.center() - start:])

        if self.size() == 1:
            self.value = values[0]
        else:
            self.value = self.left.value + self.right.value

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

def add(root: Node, until: int):
    if until < 0:
        return 0
    total = 0
    prev = None
    for node in TreeIterator(root, until):

        if prev is not None and prev.right is node:
            total += prev.left.value

        prev = node

    return total + prev.value # Add final value

def main():
    n, m, k = tuple(map(int, input().split()))
    
    values = []
    for _ in range(n):
        values.append(int(input()))

    root = Node(0, len(values), values)

    for _ in range(m + k):
        a, b, c = tuple(map(int, input().split()))

        if a == 1:
            original = values[b - 1]
            for node in TreeIterator(root, b - 1):
                node.value += c - original
                values[b - 1] = c

        if a == 2:
            print(add(root, c - 1) - add(root, b - 2))

main()

