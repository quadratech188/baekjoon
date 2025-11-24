import math
l = int(input())
a = int(input())
b = int(input())
c = int(input())
d = int(input())

print(int(l - max(math.ceil(a / c), math.ceil(b / d))))
