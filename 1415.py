n = int(input())

value_dict = {}

for _ in range(n):
    value = int(input())
    value_dict[value] = value_dict.get(value, 0) + 1


values = [(key, value) for key, value in value_dict.items()]

prime_cache = {}
def is_prime(n):
    prime_cache[n] = n > 1 and all(n % x != 0 for x in range(2, int(n ** 0.5) + 1))
    return prime_cache[n]

cache = {}
def solution(current_index, sum):
    if current_index == len(values):
        if is_prime(sum):
            return 1
        else:
            return 0

    if (current_index, sum) in cache:
        return cache[(current_index, sum)]

    result = 0

    for times_used in range(0, values[current_index][1] + 1):
        result += solution(current_index + 1, sum + values[current_index][0] * times_used)

    cache[(current_index, sum)] = result

    return result

print(solution(0, 0))
