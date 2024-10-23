n, m, k = tuple(map(int, input().split()))

matrix = []

for _ in range(n):
    matrix.append(list(map(int, input().split())))

sums = []

for i in range(n):
    row = []
    for j in range(m):
        if i < 1 and j < 1:
            row.append(matrix[i][j])
        elif i < 1:
            row.append(matrix[i][j] + row[j - 1])
        elif j < 1:
            row.append(matrix[i][j] + sums[i - 1][j])
        else:
            row.append(matrix[i][j] + row[j - 1] + sums[i - 1][j] - sums[i - 1][j - 1])
    sums.append(row)

count = 0

for x1 in range(n):
    for x2 in range(x1, n):
        while True:

        
print(count)

