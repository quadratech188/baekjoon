primes = []
for i in range(2, 542):
    for prime in primes:
        if i % prime == 0:
            continue
    primes.append(i)
