n, k = tuple(map(int, input().split()))

heights = []

for _ in range(n):
    heights.append(int(input()))

max_used = (1 << n) - 1

cache = {}

def solution(current_child, used_children):
    if used_children == max_used:
        return 1

    if (current_child, used_children) in cache:
        return cache[(current_child, used_children)]
    
    result = 0

    for next_child in range(n):
        if used_children & (1 << next_child) != 0:
            continue
        if abs(heights[next_child] - heights[current_child]) <= k:
            continue
        
        result += solution(next_child, used_children | (1 << next_child))

    cache[(current_child, used_children)] = result

    return result

print(sum(solution(i, 1 << i) for i in range(n)))
