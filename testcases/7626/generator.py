import random

n = 200000
k = 10 ** 9

print(n)

for i in range(n):
    x1 = random.randint(0, k - 1)
    x2 = random.randint(x1 + 1, k)
    print(x1, x2)
    y1 = random.randint(0, k - 1)
    y2 = random.randint(y1 + 1, k)
    print(y1, y2)
