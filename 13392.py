n = int(input())

begin = list(input())
end = list(input())


cache = {}

def solution(current):
    if current in cache:
        return cache[current]

    if current == end:
        return 0

    for index in range(n):
        current_copy = current

        for i in range(index):
            current_copy[i] = (current_copy[i] + 1) % 10
