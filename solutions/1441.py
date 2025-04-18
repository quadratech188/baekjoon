import math

n, l, r = map(int, input().split())
elements = list(map(int, input().split()))

result = 0
for subset_mask in range(1, 1 << n):
    lcm = 1
    is_valid = True
    element_count = 0
    for i in range(n):
        if (1 << i) & subset_mask:
            lcm = lcm * elements[i] // math.gcd(elements[i], lcm)
            element_count += 1
            if lcm > r:
                is_valid = False
                break
    if not is_valid:
        continue
    if element_count % 2 == 1:
        result += r // lcm - (l - 1) // lcm
    else:
        result -= r // lcm - (l - 1) // lcm

print(result)
