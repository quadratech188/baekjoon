import math
n, m = tuple(map(int, input().split()))

dimension = 2 ** math.ceil(math.log(max(n, m), 2))

image = [[0] * dimension for _ in range(dimension)]

for y in range(n):
    row = input()
    for x in range(m):
        image[y][x] = int(row[x])

cache = set()

def solution(x1, y1, x2, y2):
    if x2 - x1 == 0:
        return 1, 1
    x_center = (x1 + x2) // 2
    y_center = (y1 + y2) // 2

    color = image[x1][y1]

    if all(image[x][y] == color for x in range(x1, x2) for y in range(y1, y2)):
        return 1, 1

    corners = [
            ( (x1, y1), (x_center, y_center) ),
            ( (x1, y_center), (x_center, y2) ),
            ( (x_center, y1), (x2, y_center) ),
            ( (x_center, y_center), (x2, y2) )
            ]
    
    cost = 1
    optimized_cost = 1

    for corner in corners:
        a, b = solution(corner[0][0], corner[0][1], corner[1][0], corner[1][1])
        cost += a
        optimized_cost += b

    if tuple(tuple(row[y1:y2]) for row in image[x1:x2]) in cache:
        optimized_cost = 0
    else:
        cache.add(tuple(tuple(row[y1:y2]) for row in image[x1:x2]))

    return cost + 1, optimized_cost + 1

print(*solution(0, 0, dimension, dimension))
