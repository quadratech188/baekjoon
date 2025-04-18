import sys

n = int(input())

cost = []
for i in range(n):
    cost.append([int(x) for x in input().split()])

cache = [ [0 for k in range(1 << n)] for _ in range(n)]

ENDSTATE = (1 << n) - 1

def get_cost(start, visited):

    if visited == ENDSTATE:
        if cost[start][0] == 0:
            return sys.maxsize

        return cost[start][0]
        
    if cache[start][visited] != 0:
        return cache[start][visited]
    
    result = sys.maxsize
    for nxt in range(n):
        if cost[start][nxt] == 0:
            continue
        
        if visited & 1 << nxt != 0:
            continue

        next_visited = visited | 1 << nxt
        result = min(result, get_cost(nxt, next_visited) + cost[start][nxt])

    cache[start][visited] = result

    return result

print(get_cost(0, 1))

        
