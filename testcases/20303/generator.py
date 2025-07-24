import sys
import random

n, m, k = map(int, sys.argv[1:4])

print(n, m, k)

print(*(random.randint(1, 10000) for _ in range(n)))

for _ in range(m):
    print(random.randint(1, n), random.randint(1, n))
