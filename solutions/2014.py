import heapq

k, n = tuple(map(int, input().split()))

primes = list(map(int, input().split()))
primes.sort()

data = []

length = 1

data = list(primes)

heapq.heapify(data)

max_ = primes[-1]

for i in range(n - 1):
    k = heapq.heappop(data)
    max_ = max(max_, k)

    for p in primes:
        heapq.heappush(data, k * p)

        if k % p == 0:
            break

print(heapq.heappop(data))
