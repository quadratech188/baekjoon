def mod_add(a: int, b: int) -> int:
    return (a + b) % 1000000000

cache = {}

ALL_USED = (1 << 10) - 1

def starts_with(start: int, length: int, used) -> int:
    used = used | (1 << start)

    if length == 1:
        #print(start, length, used)
        return 1 if used == ALL_USED else 0

    if (start, length, used) in cache:
        return cache[ (start, length, used) ]

    result = 0
    if start > 0:
        result = mod_add(result, starts_with(start - 1, length - 1, used))

    if start < 9:
        result = mod_add(result, starts_with(start + 1, length - 1, used))

    cache[ (start, length, used) ] = result
    
    return result

def main(length):
    result = 0

    for i in range(1, 10):
        result = mod_add(result, starts_with(i, length, 0))

    return result
'''
a = 0
for i in range(1, 41):
    a += main(i)

print(a)
'''
print(main(int(input())))
