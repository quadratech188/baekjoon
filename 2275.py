from typing import List

class Child:
    def __init__(self, index: int, length: int):
        self.index = index
        self.length = length
        self.children: List[Child] = []

    height_cache = {}

    def height(self):
        if len(self.children) == 0: return 0

        if self.index in Child.height_cache:
            return Child.height_cache[self.index]

        Child.height_cache[self.index] = max(child.height() + child.length for child in self.children)

        return Child.height_cache[self.index]

    def solution(self, max_height) -> int:

        cost = 0
        for child in self.children:

            if child.height() + child.length <= max_height:
                continue

            elif child.height() <= max_height:
                cost += child.length - (max_height - child.height())
            else:
                cost += child.length
                cost += child.solution(max_height)

        return cost

n, h = tuple(map(int, input().split()))

children = [Child(i, 0) for i in range(n)]

for i in range(n):
    parent, length = tuple(map(int, input().split()))

    if parent == 0 and length == 0:
        root_index = i
        continue

    children[parent - 1].children.append(children[i])

    children[i].length = length

print(children[root_index].solution(h))
