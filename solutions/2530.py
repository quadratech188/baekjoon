s = list(map(int, input().split()))

t = 3600 * s[0] + 60 * s[1] + s[2] + int(input())

print((t // 3600) % 24, (t // 60) % 60, t % 60)
