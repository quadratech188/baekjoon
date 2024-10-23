n, l = tuple(map(int, input().split()))

class Ant:
    @classmethod
    def from_input(cls, index):
        return cls(int(input()), index)
    def __init__(self, value: int, index):
        if value < 0:
            self.pos = - value
            self.direction = 'left'
        else:
            self.pos = value
            self.direction = 'right'

        self.index = index

    def time(self):
        if self.direction == 'right':
            return l - self.pos
        else:
            return self.pos

ants = []

for i in range(n):
    ants.append(Ant.from_input(i))

max_ant = None
max_value = 0
left = 0
right = 0
for ant in ants:
    if ant.direction == 'left':
        left += 1
    else:
        right += 1
    if ant.time() > max_value:
        max_ant = ant
        max_value = ant.time()

if max_ant.direction == 'right':
    ant_index = left
else:
    ant_index = left - 1


ants.sort(key=lambda x: x.pos)

print(ants[ant_index].index + 1, max_value)
