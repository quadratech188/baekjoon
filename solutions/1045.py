from typing import List, Set, Tuple

n, m = tuple(map(int, input().split()))

connections = []

for _ in range(n):
    connections.append(
        list(map(lambda x: True if x == 'Y' else False, input()))
    )

edges = [(i, j) for i in range(n) for j in range(i + 1, n) if connections[i][j]]

parents = list(range(0, n))

def get_parent(x):
    while parents[x] != x:
        x = parents[x]

    return x

def mark_connected(x, y):
    x= get_parent(x)
    y= get_parent(y)
    if x < y:
        parents[y] = x
    else:
        parents[x] = y

connection_counts = [0] * n

unused = []

count = 0

for i, j in edges:
    if get_parent(i) != get_parent(j):
        mark_connected(i, j)
        connection_counts[i] += 1
        connection_counts[j] += 1
        count += 1
    else:
        unused.append((i, j))

if count != n - 1:
    print(-1)
elif len(edges) < m:
    print(-1)
else:
    for index in range(0, m - count):
        i, j = unused[index]
        connection_counts[i] += 1
        connection_counts[j] += 1

    print(*connection_counts)
