n = int(input())

costs = []

for _ in range(n):
    costs.append(list(map(int, input().split())))

max_jobs = 2 << n - 1

cache = {}

def solution(current_index, current_jobs):
    if current_index == n:
        return 0

    if (current_index, current_jobs) in cache:
        return cache[(current_index, current_jobs)]

    cache[(current_index, current_jobs)] = min(costs[current_index][job] + solution(current_index + 1, current_jobs | (1 << job)) for job in filter(lambda x: (1 << x) & current_jobs == 0, range(0, n)))

    return cache[(current_index, current_jobs)]

print(solution(0, 0))

        
