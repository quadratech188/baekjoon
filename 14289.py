MOD = 1_000_000_007

from modules.Matrix import Matrix

n, m = tuple(map(int, input().split()))

mult = Matrix([[0 for _ in range(n)] for _ in range(n)], MOD)

for _ in range(m):
    a, b = tuple(map(int, input().split()))

    mult.values[a-1][b-1] = 1
    mult.values[b-1][a-1] = 1

D = int(input())


initial = Matrix([[0] * n], MOD)
initial.values[0][0] = 1

print((initial * (mult ** D)).values[0][0])
