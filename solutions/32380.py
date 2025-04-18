n, u, d = tuple(map(int, input().split()))

a = list(map(int, input().split()))
b = list(map(int, input().split()))

sum = 0

total_sum = 0

for i in range(n):
    a[i] += u * (n - 1 - i)
    b[i] -= d * (n - 1 - i)

print(a)
print(b)
