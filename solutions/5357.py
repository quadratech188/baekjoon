for _ in range(int(input())):
    s = input()

    prev_c = '\0'
    for c in s:
        if prev_c != c:
            print(c, end='')
        prev_c = c
    print()
