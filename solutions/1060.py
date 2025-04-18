class Range:
    def __init__(self, left: int, right: int):
        self.left = left
        self.right = right
        if self.left is None:
            self.range_type = 'r'
        elif self.right is None:
            self.range_type = 'l'
        else:
            self.range_type = 'lr'

        if self.range_type != 'r':
            self.current = self.left
            self.current_side = 'l'
        else:
            self.current = self.right
            self.current_side = 'r'

l = int(input())
numbers = list(map(int, input().split()))
n = int(input())

numbers.sort()

ranges = [(0, numbers[0], numbers[0], 'right')]

for i in range(len(numbers) - 1):
    ranges.append((numbers[i], numbers[i + 1], numbers[i], 'left'))

ranges.append((numbers[-1], 0, numbers[-1], 'left']))

print(ranges)
