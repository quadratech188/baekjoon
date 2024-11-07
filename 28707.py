import heapq

n = int(input())

a = tuple(map(int, input().split()))

m = int(input())

actions = []
for _ in range(m):
    actions.append(tuple(map(int, input().split())))

Graph = {}

start = a

end = tuple(sorted(a))

def apply(action, perm):
    result = list(perm)
    result[action[0] - 1] = perm[action[1] - 1]
    result[action[1] - 1] = perm[action[0] - 1]
    return tuple(result)

heap = []

distances = {
        end: 0
        }

heapq.heappush(heap, (0, end))

while (len(heap) != 0):
    current = heapq.heappop(heap)

    if current[1] in distances and current[0] > distances[current[1]]:
        continue

    for action in actions:
        next = apply(action, current[1])

        if next not in distances or current[0] + action[2] < distances[next]:
            distances[next] = current[0] + action[2]
            heapq.heappush(heap, (distances[next], next))


if start not in distances:
    print(-1)
else:
    print(distances[start])

