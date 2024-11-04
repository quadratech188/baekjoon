class ListSum:
    def __init__(self, value, lis):
        self.value = value
        self.lis = lis

    def __add__(self, other):
        return ListSum(self.value + other.value, self.lis + other.lis)



cache = {}
class Person:

    def __init__(self, index, value, children = []):
        self.index = index
        self.value = value
        self.children = children

    def max_value(self, used_parent):
        if self.index in cache:
            return cache[self.index]

        if self.children == []:
            return ListSum(self.value, [self.value])
        
        if used_parent:
            cache[self.index] =  sum(max(child.max_value(False), 0) for child in self.children)

        else:
            cache[self.index] = sum(max(child.max_value(False), 0) for child in self.children), max(self.value) + sum(max(child.max_value(True) for child in self.children))

        return cache[self.index]

n = int(input())

values = list(map(int, input().split()))

indices = list(map(int, input().split()))

people = [Person(index, value) for index, value in enumerate(values)]

for i, index in enumerate(indices):
    people[i + 1].children.append(people[index - 1])

print(people[0].max_value(False))
