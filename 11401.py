MOD = 1_000_000_007

def factorial(n: int, MOD: int):
    result = 1

    for i in range(1, n + 1):
        result = (result * i) % MOD

    return result


def power(n: int, p: int, MOD: int):
    cache = {}
    cache[1] = n

    power_of_2 = 2

    if p % 2 == 1:
        result = n
        p -= 1
    else:
        result = 1

    while p > 0:
        cache[power_of_2] = (cache[power_of_2 // 2] ** 2) % MOD

        if p % (2 * power_of_2) != 0:
            result = (result * cache[power_of_2]) % MOD
            p -= power_of_2

        power_of_2 *= 2

    return result

def divide(n, m, MOD):
    return (n * power(m, MOD - 2, MOD)) % MOD

n, k = tuple(map(int, input().split()))

top = factorial(n, MOD)

bottom = factorial(k, MOD) * factorial(n - k, MOD) % MOD

print(divide(top, bottom, MOD))
