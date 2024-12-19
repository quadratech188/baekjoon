def solution(a):
    sum = 0
    for index in range(64):
        if a & (1 << index):
            sum += a // (2 << index) * (1 << index) + a % (1 << index) + 1
        else:
            sum += a // (2 << index) * (1 << index)
    return sum

a, b = tuple(map(int, input().split()))

print(solution(b) - solution(a - 1))
