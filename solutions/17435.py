MAX = 20

m = int(input())
data = list(map(int, input().split()))

cache = [[0] * m for _ in range(MAX + 1)]

for i in range(m):
    cache[0][i] = data[i]

for i in range(1, MAX):
    for j in range(m):
        temp = cache[i - 1][j]
        cache[i][j] = cache[i - 1][temp - 1]

def solution(steps, start):
    current_position = start

    for k in range(MAX):
        if steps & (1 << k):
            current_position = cache[k][current_position - 1]

    return current_position

q = int(input())
for i in range(q):
    n, x = tuple(map(int, input().split()))
    print(solution(n, x))
