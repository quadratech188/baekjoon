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

def get_euler_pi(n):
    if n == 1:
        return 0

    factors = factorize(n)

    for factor in factors:
        n = int( n / factor * (factor - 1) )

    return n
cache = {}
def main():
    n, k = tuple(map(int, input().split()))

    print(int(get_euler_pi(n) * k * (n * k / 2)))
main()
