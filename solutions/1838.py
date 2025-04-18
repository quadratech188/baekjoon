n = int(input())
values = list(map(int, input().split()))
map = {j: i for i, j in enumerate(sorted(values))}
max_ = 0
for i, j in enumerate(values): max_ = max(max_, i - map[j])
print(max_)
