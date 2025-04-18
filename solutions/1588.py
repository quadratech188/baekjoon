cache = {}

lookup = {
        1: (1, 3, 2),
        2: (2, 1, 1),
        3: (2, 3, 2)
        }

def solution(i, right, order):
    if right < 0:
        return (0, 0, 0)
    
    if right > 3 ** order - 1:
        right = 3 ** order - 1

    if order == 0:
        if i == 1:
            return (1, 0, 0)
        elif i == 2:
            return (0, 1, 0)
        else:
            return(0, 0, 1)

    if (i, right, order) in cache:
        return cache[(i, right, order)]

    a = solution(lookup[i][0], right, order - 1)
    b = solution(lookup[i][1], right - 3 ** (order - 1), order - 1)
    c = solution(lookup[i][2], right - 2 * 3 ** (order - 1), order - 1)

    cache[(i, right, order)] = (
            a[0] + b[0] + c[0],
            a[1] + b[1] + c[1],
            a[2] + b[2] + c[2]
            )

    return cache[(i, right, order)]

i = int(input())
left = int(input())
right = int(input())
n = int(input())

a = solution(i, right, n)
b = solution(i, left - 1, n)
print(a[0] - b[0], a[1] - b[1], a[2] - b[2])

