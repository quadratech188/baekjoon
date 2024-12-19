n, s = tuple(map(int, input().split()))

values = list(map(int, input().split()))

partial = [0]

for value in values:
    partial.append(value + partial[-1])

def f(a, b):
    return partial[b + 1] - partial[a]

a = 0
b = 0

l = float('inf')

while True:
    if a == n or b == n: break
    if f(a, b) < s:
        b += 1
    else:
        l = min(l, b - a + 1)
        if f(a + 1, b) < s:
            b += 1
        else:
            a += 1

if l == float('inf'):
    print(0)
else:
    print(l)
