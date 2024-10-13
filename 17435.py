m = int(input())

values = list(map(int, input().split()))

loop_counts = [0 for _ in range(m)]

for i in range(m):
    if loop_counts[i] != 0:
        continue

    used = set()

    used.add(i)

    loop_count = 1
    
    temp = i
    while True:
        temp = values[temp] - 1
        if temp in used:
            break
        loop_count += 1
        used.add(temp)

    for t in used:
        loop_counts[t] = loop_count

print(loop_counts)

q = int(input())

for _ in range(q):
    n, x = tuple(map(int, input().split()))

    result = x - 1
    for _ in range(n % values[x - 1]):
        result = values[result] - 1

    print(result + 1)
