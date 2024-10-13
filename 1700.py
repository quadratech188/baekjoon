n, k = tuple([int(x) for x in input().split()])

names = [int(x) for x in input().split()]

cache = set()

last_used = [0] * (len(names) + 1)

for reversed_index, value in enumerate(reversed(names)):
    index = len(names) - 1 - reversed_index
    if value not in cache:
        cache.add(value)
        last_used[value] = index

print(last_used)
multi_tap = set()

count = 0

for index, value in enumerate(names):
    print(multi_tap)
    if value in multi_tap:
        continue

    if len(multi_tap) < n:
        multi_tap.add(value)
        continue
   
    removed_value = min(multi_tap, key = lambda x: last_used[x])
    multi_tap.remove(removed_value)
    multi_tap.add(value)
    count += 1

print(count)
