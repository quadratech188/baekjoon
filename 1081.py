
def value(n: int) -> int:
    place = 1
    result = 0

    while place <= n:
        before = n // (place * 10)
        current_digit = (n // place) % 10
        after = n % place

        result += before * place * 45 
        result += current_digit * (after + 1) 
        result += (current_digit * (current_digit - 1)) // 2 * place

        place *= 10

    return result

l, u = tuple(map(int, input().split()))
print(value(u) - value(l - 1))

