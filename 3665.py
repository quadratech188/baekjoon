from collections import deque

def main():
    n = int(input())

    matrix = [[False] * n for _ in range(n)]

    positions = list(map(int, input().split()))

    for i in range(n):
        for j in range(i + 1, n):
            matrix[positions[i] - 1][positions[j] - 1] = True

    m = int(input())

    for _ in range(m):
        a, b = tuple(map(int, input().split()))
        a -= 1
        b -= 1

        if matrix[a][b] == False:
            matrix[a][b] = True
            matrix[b][a] = False
        else:
            matrix[a][b] = False
            matrix[b][a] = True

    dependencies = [0] * n

    for parent in range(n):
        for child in range(n):
            if matrix[parent][child]:
                dependencies[child] += 1

    queue = deque()

    for i in range(n):
        if dependencies[i] == 0:
            queue.append(i)

    maybe = False

    result = []

    for _ in range(n):
        if len(queue) == 0:
            print('IMPOSSIBLE')
            return
        if len(queue) >= 2:
            maybe = True

        parent = queue.pop()

        result.append(parent + 1)

        for child in range(n):
            if not matrix[parent][child]: continue

            dependencies[child] -= 1

            if dependencies[child] == 0:
                queue.append(child)

    if maybe:
        print('?')
    else:
        print(*result)

for _ in range(int(input())):
    main()
