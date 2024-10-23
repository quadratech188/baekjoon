from collections import defaultdict

n = int(input())

sums = [[0] * n for _ in range(n)]

def idx(list, x, y):
    if x < 0 or y < 0:
        return 0

    return list[x][y]

for i in range(n):
    row = list(map(int, input().split()))

    for j in range(n):
        sums[i][j] = idx(sums, i - 1, j) + idx(sums, i, j -1) - idx(sums, i - 1, j - 1) + row[j]


def get_profit(x1, y1, x2, y2):
    return idx(sums, x2 - 1, y2 - 1) + idx(sums, x1 - 1, y1 - 1) - idx(sums, x1 - 1, y2 - 1) - idx(sums, x2 - 1, y1 - 1)

count = 0

'''
    x1  x2  x3
y1   +---+---+
     | A | B |
y2   +---+---+
     | C | D |
y3   +---+---+
'''


for x2 in range(1, n):
    for y2 in range(1, n):

        A = defaultdict(int)
        B = defaultdict(int)

        for x1 in range(0, x2):
            for y1 in range(0, y2):
                A[get_profit(x1, y1, x2, y2)] += 1

        for x3 in range(x2 + 1, n + 1):
            for y1 in range(0, y2):
                B[get_profit(x2, y1, x3, y2)] += 1

        for x3 in range(x2 + 1, n + 1):
            for y3 in range(y2 + 1, n + 1):
                count += A[get_profit(x2, y2, x3, y3)]

        for x1 in range(0, x2):
            for y3 in range(y2 + 1, n + 1):
                count += B[get_profit(x1, y2, x2, y3)]

print(count)
