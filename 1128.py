n = int(input())

# (weight, cost)

items = []

for _ in range(n):
    items.append(tuple(map(int, input().split())))

items.sort()

c = int(input())

used = [False] * n

def solution(start = 0, weight = 0, cost = 0):
    if weight > c:
        return 0

    if start == len(items):
        return cost

    return max(
            solution(start + 1, weight, cost),
            solution(start + 1, weight + items[start][0], cost + items[start][1])
            )

print(solution(0, 0, 0))
