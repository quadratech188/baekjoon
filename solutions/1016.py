a, b = map(int, input().split())

values = [1] * (b - a + 1)

for divisor in range(2, 1000001):
    p = divisor ** 2
    start = ((a - 1) // p + 1) * p

    for value in range(start, b + 1, p):
        values[value - a] = 0

print(sum(values))
