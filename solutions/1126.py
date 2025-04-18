n = int(input())

blocks = list(map(int, input().split()))

cache = {}

def solution(current_index, height_difference):
    if (current_index, height_difference) in cache:
        return cache[(current_index, height_difference)]

    if current_index == len(blocks):
        if height_difference == 0:
            return 0
        else:
            return -1000000

    a = solution(current_index + 1, height_difference)
    b = blocks[current_index] + solution(current_index + 1, height_difference - blocks[current_index]);
    c = solution(current_index + 1, height_difference + blocks[current_index]);

    cache[(current_index, height_difference)] = max(a, b, c)
    return max(a, b, c)

t = solution(0, 0)
if t <= 0:
    print(-1)
else:
    print(t)

