def flip(n):
    result = 0
    while n != 0:
        result = 10 * result + n % 10
        n //= 10

    return result

d = int(input())

if d % 9 != 0:
    print(-1)
else:
    for x in range(1_000_000):
        if x - flip(x) == d:
            print(x)
            break

    else:
        print(-1)
