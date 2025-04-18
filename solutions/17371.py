import math


n = int(input())

points = []
for _ in range(n):
    points.append(tuple(map(int, input().split())))

min_distance2 = math.inf

value = None

for i in range(n):
    a = None
    b = None
    max_distance2 = 0
    for j in range(n):
        if i == j: continue
        max_distance2 = max(max_distance2, (points[i][0] - points[j][0]) ** 2 + (points[i][1] - points[j][1]) ** 2)

    if max_distance2 < min_distance2:
        value = points[i]
        min_distance2 = max_distance2

print(*value)
