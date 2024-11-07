string = input()

palindrome_cache = [[None] * (len(string) + 1) for _ in range(len(string) + 1)]

def is_palindrome(start, end):
    if palindrome_cache[start][end] is not None:
        pass

    elif end - start == 1:
        palindrome_cache[start][end] = True

    elif end - start == 2:
        palindrome_cache[start][end] = string[start] == string[end - 1]

    else:
        palindrome_cache[start][end] = string[start] == string[end - 1] and is_palindrome(start + 1, end - 1)

    return palindrome_cache[start][end]

divisions_until = [10000]  * len(string)

divisions_until[0] = 1

for until in range(len(string)):
    if is_palindrome(0, until + 1):
        divisions_until[until] = 1
        continue

    for divider in range(0, until):
        if is_palindrome(divider + 1, until + 1):
            divisions_until[until] = min(divisions_until[until], divisions_until[divider] + 1)

print(divisions_until[len(string) - 1])
