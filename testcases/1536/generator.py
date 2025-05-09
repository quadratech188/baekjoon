import sys
import random

N = int(sys.argv[1])
K = int(sys.argv[2])

print(f"{N} {K}")
for _ in range(N):
    row = ''.join(random.choice('01') for _ in range(N))
    print(row)
