

MOD = 1_000_000_000


a, b = tuple(map(int, input().split()))

mult = Matrix([
    [0, 1],
    [1, 1]
], MOD)

initial = Matrix([
    [1, 1]
], MOD)

end = initial * (mult ** b)

start = initial * (mult ** (a - 1))

print((end.values[0][1] - start.values[0][1]) % MOD)
