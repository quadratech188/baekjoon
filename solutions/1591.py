n, m = tuple(map(int, input().split()))

children = {}

not_child = set(range(1, n + 1))

for _ in range(n - m + 1):
    values = list(map(int, input().split()))

    for index in range(1, len(values)):
        if values[index - 1] not in children:
            children[values[index - 1]] = [values[index]]
        else:
            children[values[index - 1]].append(values[index])

        not_child -= set({values[index]})

print(children)

for head in not_child:
    next = head

    while next in children:
        print(next, end = ' ')
        next = children[next].pop(0)
        next = children[next_]

    print(next, end = ' ')
