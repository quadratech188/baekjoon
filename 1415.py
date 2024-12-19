n = int(input())

value_dict = {}

for _ in range(n):
    appeared = int(input())
    value_dict[appeared] = value_dict.get(appeared, 0) + 1

prime_cache = {}
def is_prime(n):
    if n in prime_cache:
        return prime_cache[n]
    prime_cache[n] = n > 1 and all(n % x != 0 for x in range(2, int(n ** 0.5) + 1))
    return prime_cache[n]

cache = [0] * (n * 10000)

if 0 in value_dict.keys():
    cache[0] = value_dict[0] + 1
    del value_dict[0]
else:
    cache[0] = 1


for cost, appeared in value_dict.items():
    for current in range(len(cache) - 1, -1, -1):
        for i in range(1, appeared + 1):
            if current - i * cost < 0: break
            cache[current] += cache[current - i * cost]
sum = 0
for key, times in enumerate(cache):
    if is_prime(key):
        sum += times;

print(sum)
