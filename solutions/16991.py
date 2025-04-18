import sys

n = int(input())

positions = []

for i in range(n):
    positions.append(tuple(map(int, input().split())))

cost = []
for i in range(n):
    cost.append([])
    for j in range(n):
        cost[i].append( ( (positions[i][0] - positions[j][0]) ** 2 + (positions[i][1] - positions[j][1]) ** 2 ) ** 0.5 )

cache = [ [0 for _ in range(1 << n)] for _ in range(n)]

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

 
