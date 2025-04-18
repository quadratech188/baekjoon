n, c = tuple(map(int, input().split()))

values = list(map(int, input().split()))

center = n // 2

l_cache = {}
def left(index, currentValue):
    if index == center:
        if currentValue not in l_cache:
            l_cache[currentValue] = 0
        l_cache[currentValue] += 1
        return
    left(index + 1, currentValue + values[index])
    left(index + 1, currentValue)

r_cache = {}
def right(index, currentValue):
    if index == c:
        if currentValue not in l_cache:
            l_cache[currentValue] = 0
        l_cache[currentValue] += 1
        return
    right(index + 1, currentValue + values[index])
    right(index + 1, currentValue)

left(0, 0)
right(center, 0)

sum = 0

print(l_cache)
print(r_cache)

for l, lt in l_cache.items():
    for r, rt in r_cache.items():
        if l + r > c: continue
        sum += lt * rt

print(sum)
