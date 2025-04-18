t = int(input())
for _ in range(t):
    n, m = tuple(map(int, input().split()))


    result = 1

    for i in range(1, n + 1):
        result = result * (m - i + 1) // i

    print(result)
