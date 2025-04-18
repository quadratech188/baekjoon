n, p, k = tuple(map(int, input().split()))

values = [[0] * n for _ in range(n)]

for _ in range(p):
    a, b, c = tuple(map(int, input().split()))

    values[a - 1][b - 1] = c
