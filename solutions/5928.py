def from_time(x):
    return 1440 * x[0] + 60 * x[1] + x[2]

t = from_time(list(map(int, input().split()))) - from_time([11, 11, 11])

print(t if t >= 0 else -1)
