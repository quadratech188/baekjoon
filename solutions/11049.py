n = int(input())

values = []
for _ in range(n):
    values.append(tuple(map(int, input().split())))

cache = {}
def solution(start, end):
    if end - start == 1:
        cache[(start, end)] = 0
        return 0
    if (start, end) in cache:
        return cache[(start, end)]
    min_ = float('inf')
    for i in range(start + 1, end):
        min_ = min(min_, solution(start, i) + solution(i, end) + values[start][0] * values[i][0] * values[end - 1][1])

    cache[(start, end)] = min_
    return min_

print(solution(0, n))
