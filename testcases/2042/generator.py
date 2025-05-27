import sys
import random

n = int(sys.argv[1])
m = int(sys.argv[2])
k = int(sys.argv[3])

print(n, m, k)

for _ in range(n):
    print(random.randint(-1<<32, 1<<32))

actions = []

for _ in range(m):
    actions.append((1, random.randint(1, n), random.randint(-1<<32, 1<<32)))

for _ in range(k):
    b = random.randint(1, n)
    c = random.randint(b, n)

    actions.append((2, b, c))

import numpy as np

random.shuffle(actions)

for action in actions:
    print(*action)
