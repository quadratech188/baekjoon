for _ in range(1):
    s, d = map(int, input().split())
    if (s + d) % 2 != 0 or s < d:
        print(-1)
        continue

    print((s + d) // 2, (s - d) // 2)
