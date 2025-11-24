l, p = map(int, input().split())

print(*map(lambda x: x - l * p, map(int, input().split())))
