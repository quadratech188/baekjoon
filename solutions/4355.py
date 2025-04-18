from typing import Set

def factorize(n: int) -> Set[int]:
    prime = 2
    
    primes = set()
    while n != 1:
        if prime ** 2 > n:
            primes.add(n)
            break

        if n % prime == 0:
            n = int(n / prime)
            primes.add(prime)
        else:
            prime += 1

    return primes
while True:
    n = int(input())
    if n == 0:
        break
    if n == 1:
        print(0)
        continue

    factors = factorize(n)

    for factor in factors:
        n = int( n / factor * (factor - 1) )

    print(n)
