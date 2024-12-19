n = int(input())

a = []
b = []

if n % 2 == 0:
    for i in range (1, n // 2):
        a.append(i)
        a.append(n + 1 - i)
