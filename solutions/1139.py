from itertools import combinations
import math

n = int(input())

lengths = list(map(int, input().split()))
lengths.sort()

max_bitmap = (1 << n) - 1

def area(a, b, c):
    p = (a + b + c) / 2
    return math.sqrt( p * (p - a) * (p - b) * (p - c) )

cache = {}
def solution(used_bitmap: int):
    if used_bitmap == max_bitmap:
        return 0.0

    if used_bitmap in cache:
        return cache[used_bitmap]
    
    result = 0.0

    for a, b, c in combinations(filter(lambda x: used_bitmap & (1 << x) == 0, range(0, n)), 3):
        if lengths[a] + lengths[b] <= lengths[c]:
            continue

        new_bitmap = used_bitmap | (1 << a) | (1 << b) | (1 << c)

        result = max(solution(new_bitmap) + area(lengths[a], lengths[b], lengths[c]), result)

    cache[used_bitmap] = result
    return result

print('{:.14f}'.format(solution(0)))

