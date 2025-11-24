print('Gnomes:')
for _ in range(int(input())):
    v = list(map(int, input().split()))

    if v[0] <= v[1] <= v[2] or v[0] >= v[1] >= v[2]:
        print('Ordered')
    else:
        print('Unordered')
