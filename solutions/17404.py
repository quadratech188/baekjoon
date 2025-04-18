n = int(input())

costs = []

for _ in range(n):
    costs.append(tuple(map(int, input().split())))

cache = {}
def solution(first, current, color):
    if (first, current, color) in cache:
        return cache[(first, current, color)]

    if current == n:
        return 0

    min_ = float('inf')
    for i in range(3):
        if i == color: continue
        if current == n - 2 and first == i: continue

        min_ = min(min_, solution(first, current + 1, i))

    cache[(first, current, color)] = costs[current][color] + min_

    return cache[(first, current, color)]

print(min(solution(0, 0, 0), solution(1, 0, 1), solution(2, 0, 2)))
