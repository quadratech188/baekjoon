import sys
n = int(input())

sys.setrecursionlimit(300000)

values = list(map(int, input().split()))

managers = list(map(int, input().split()))

children = {}

for child, parent in enumerate(managers):
    if parent - 1 not in children:
        children[parent - 1] = []

    children[parent - 1].append(child + 1)

cache = {}

def solution(index, self_used):
    if index not in children:
        if self_used:
            return values[index], [index]
        else:
            return 0, []

    if (index, self_used) in cache:
        return cache[(index, self_used)]

    sum = 0
    indices = []

    if not self_used:
        for child in children[index]:
            used_value, used_indices = solution(child, True)
            not_used_value, not_used_indices = solution(child, False)
            if used_value > not_used_value:
                if used_value > 0:
                    sum += used_value
                    indices += used_indices
            elif not_used_value > 0:
                sum += not_used_value
                indices += not_used_indices

    else:
        sum += values[index]
        indices.append(index)
        for child in children[index]:
            not_used_value, not_used_indices = solution(child, False)
            if not_used_value > 0:
                sum += not_used_value
                indices += not_used_indices
    
    cache[(index, self_used)] = (sum, indices)

    return sum, indices

n_value, n_indices = solution(0, False)
y_value, y_indices = solution(0, True)
print(y_value, n_value)
print(*sorted(map(lambda x: x + 1, y_indices)), -1)
print(*sorted(map(lambda x: x + 1, n_indices)), -1)
