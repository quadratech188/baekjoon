n = int(input())

values = []

for _ in range(n):
    values.append(int(input()))

map = {j:i for i, j in enumerate(sorted(values))}

used = set()

result = 0

for value in values:
    if value in used:
        continue

    if value == values[map[value]]:
        continue

    current = value

    loop = set()

    while value not in used:
        loop.add(value)
        used.add(value)
        value = values[map[value]]

    result += min(
            sum(loop) + min(loop) * (len(loop) - 2),
            sum(loop) + min(loop) + min(values) * (len(loop) + 1)
            )

print(result)
