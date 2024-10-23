import sys

input = sys.stdin.readline

INF = 10000
NIL = 20000
a = input()
b = input()

len_a = len(a)
len_b = len(b)

cache = [[NIL] * 1000 for _ in range(1000)]

def distance(index_a, index_b):
    if index_a == len_a:
        if index_b == len_b:
            return 0
        else:
            return 1

    if cache[index_a][index_b] != NIL:
        return [index_a][index_b]

    result = INF

    for next_index_b in range(index_b, len_b):
        if a[index_a] != b[next_index_b]:
            continue

        next_distance = distance(index_a + 1, next_index_b + 1)

        if next_index_b == index_b:
            result = min(result, next_distance)
        else:
            result = min(result, next_distance + 1)
    
    cache[index_a][index_b] = result
    return result

result = distance(0, 0)

if result == INF:
    print(-1)
else:
    print(result)
