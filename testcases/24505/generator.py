import sys
import random

N = int(sys.argv[1])

print(N)

print(*(random.randint(1, N) for _ in range(N)))
