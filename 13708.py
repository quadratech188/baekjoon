
n = int(input())

points = []

for _ in range(n):
    points.append(tuple(map(float, input().split())))

center = (0, 0)

give = 0.1

while True:
    max_point = None
    max_distance = 0

    for point in points:
        if (center[0] - point[0]) ** 2 + (center[1] - point[1]) ** 2 >= max_distance:
            max_point = point
            max_distance = (center[0] - point[0]) ** 2 + (center[1] - point[1]) ** 2
    
    center = (
            center[0] + give * (max_point[0] - center[0]),
            center[1] + give * (max_point[1] - center[1])
            )

    give *= 0.999

    if give < 0.000000001: break

print('{:.2f}'.format(2 * max_distance ** 0.5))
