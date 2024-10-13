# 비용은 항상 일정하다

n = int(input())

costs = list(map(int, input().split()))
factorial = 1
for i in range(2, n + 1):

    factorial = (i * (i - 1) * factorial) % 1000000007

sum = 0
cost = 0
for i in range(n):
    cost += sum * costs[i]
    sum += costs[i]

print(cost)
print(factorial)
