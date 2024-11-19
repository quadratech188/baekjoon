n, c = tuple(map(int, input().split()))

values = list(map(int, input().split()))

center = n // 2

cache = {}

def left(index, currentValue):
    if index == center:
        if currentValue not in cache:
            cache[currentValue] = 0
        cache[currentValue] += 1
        return
    left(index + 1, currentValue + values[index])
    left(index + 1, values[index])

def right(index, maxValue):
    if index == c:
        if maxValue >= 0:
            return 1
        else:
            return 0

    if maxValue < 0:
        return 0

    return right(index + 1, maxValue) + right(index + 1, maxValue - values[index])

left(0, 0)

result = 0

for i, j in cache.items():
    result += j * right(center, n - i)

print(result)

