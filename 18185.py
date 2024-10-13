n = int(input())
a_i = input().split()

a_i = [int(k) for k in a_i]

def sub(index, length, by):
    for i in range(index, index + length):
        a_i[i] -= by

sum = 0

for i in range(n - 2):
    sum += 7 * min(a_i[i:i+3])
    sub(i, 3, min(a_i[i:i+3]))

for i in range(n - 1):
    sum += 5 * min(a_i[i:i+2])
    sub(i, 2, min(a_i[i:i+2]))

for i in range(n):
    sum += 3 * min(a_i[i:i+1])
    sub(i, 1, min(a_i[i:i+1]))

print(sum)
