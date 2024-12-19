n = int(input()) // 2

for i in range(1, n + 1):
    k = 0

    for j in range(2 * n + 1):
        if j % 2 == 0:
            k += i
        else:
            k += n - i

        if k > 2 * n + 1:
            k -= (2 * n + 1)

        print(k, end=' ')

    print()

'''
1 2 3 4 5 6 7 8 9
5 2 7 8 5 1 9 8 4
'''
