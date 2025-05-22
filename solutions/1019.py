result = [0] * 10

def solution(n, power_of_10, include_zeros):
    if power_of_10 == -1: return
    mod = 10 ** power_of_10
    for i in range(1, n // mod):
        result[i] += mod

    if include_zeros and n // mod > 0:
        result[0] += mod

    for i in range(10):
        result[i] += (n // mod) * power_of_10 * mod // 10

    if n // mod > 0 and not include_zeros:
        result[0] -= (mod - 1) // 9

    if n // mod > 0 or include_zeros:
        result[n // mod] += n % mod + 1

    solution(n % mod, power_of_10 - 1, include_zeros or n // mod != 0)

n = int(input())

solution(n, 20, False)

print(*result)
