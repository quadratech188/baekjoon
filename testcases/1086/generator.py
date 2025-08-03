import random

N = 15
K = random.randint(1, 100)

print(N)

for _ in range(N):
    print(random.randint(1, 10 ** random.randint(1, N)))

print(K)
