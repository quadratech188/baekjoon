cache = {}
def size(n):
    if n == 0:
        return 1
    if n == 1:
        return 1
    if n in cache:
        return cache[n]

    cache[n] = size(n - 1) + size(n - 2) + 1

    return cache[n]

def shortest_path(n, end) -> str:
    if end == 1:
        return ''
    if n == 1:
        return ''
    if n == 2:
        if end == 2:
            return 'L'
        else:
            return 'R'
    
    if end <= size(n - 2) + 1:
        return 'L' + shortest_path(n - 2, end - 1)
    else:
        return 'R' + shortest_path(n - 1, end - size(n - 2) - 1)

n, start, end = tuple(map(int, input().split()))
if start != end:
    start_path = shortest_path(n, start)
    end_path = shortest_path(n, end)

    while len(start_path) and len(end_path) and start_path[0] == end_path[0]:
        start_path = start_path[1:]
        end_path = end_path[1:]

    print('U' * len(start_path) + end_path)

