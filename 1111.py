import math
n = int(input())
seq = list(map(int, input().split()))

if n == 1:
    print('A')
    quit()

if n == 2:
    if seq[0] == seq[1]:
        print(seq[0])
    else:
        print('A')
    quit()

a = 0
if seq[1] != seq[0]:
    a = int((seq[2] - seq[1]) / (seq[1] - seq[0]))

b = seq[1] - a * seq[0]

for i in range(n - 1):
    if seq[i] * a + b != seq[i + 1]:
        print('B')
        quit()

print(seq[n - 1] * a + b)
