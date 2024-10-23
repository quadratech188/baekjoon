n = int(input())

values = set()

for _ in range(n):
    values.add(int(input()))
max_value = 0
for angle in range(1, 360):
    new_values = set(map(lambda x: (x + angle) % 360, values))

    max_value = max(max_value, len(new_values & values))

print(max_value * 2)
