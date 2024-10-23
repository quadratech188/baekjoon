class Tank:
    @classmethod
    def from_input(cls):
        return cls(*tuple(map(int, input().split())))
    def __init__(self, y, height, width, length):
        self.y = y
        self.height = height
        self.area = width * length

    def volume(self, water_height):
        if water_height < self.y:
            return 0
        if water_height > self.y + self.height:
            return self.area * self.height
        else:
            return self.area * (water_height - self.y)

tanks = []

n = int(input())

for _ in range(n):
    tanks.append(Tank.from_input())

volume = int(input())

start = 0
end = 1_040_000

if sum(tank.volume(end) for tank in tanks) < volume:
    print('OVERFLOW')
else:
    for _ in range(40):
        middle = (start + end) / 2
        if sum(tank.volume(middle) for tank in tanks) >= volume:
            end = middle
        else:
            start = middle

        #print(middle)

    print('{:.2f}'.format(middle))
