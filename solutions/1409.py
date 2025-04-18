n = int(input())

values = set()

for _ in range(n):
    values.add(int(input()))

result = 0

for angle in range(1, 360):
    colors = [0] * 360
    for k in values:
        if k + angle in values:
            if colors[k] == 0:
                colors[k] = 1
                colors[k + angle] = 2

    cnt = 0
    for i in range(360):
        if colors[i] != 0:
            cnt += 1

    result = max(result, cnt)

print(result)
