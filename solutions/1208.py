n, s = tuple(map(int, input().split()))

values = list(map(int, input().split()))

total = 0

times_appeared = {}

def right(start: int, sum: int):
    if start == n:
        times_appeared[sum] = times_appeared.get(sum, 0) + 1
        return

    right(start + 1, sum)
    right(start + 1, sum + values[start])

def left(end: int, sum: int):
    if end == -1:
        if s - sum in times_appeared:
            global total
            total += times_appeared[s - sum]
        return

    left(end - 1, sum)
    left(end - 1, sum + values[end])

middle = int(n / 2)

right(middle, 0)
left(middle - 1, 0)

if s == 0:
    print(total - 1)
else:
    print(total)

