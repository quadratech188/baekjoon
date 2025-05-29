import sys
import random

n = int(sys.argv[1])
m = int(sys.argv[2])

print(n)

for _ in range(n):
    print(random.randint(1, 10 ** 9))

print(m)

for _ in range(m):
    i = random.randint(1, n)
    j = random.randint(i, n)
    print(i, j, random.randint(1, 10 ** 9))
