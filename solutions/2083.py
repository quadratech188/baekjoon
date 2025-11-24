while True:
    s = input().split()
    if s == ['#', '0', '0']: break

    print(s[0], 'Senior' if int(s[1]) > 17 or int(s[2]) >= 80 else 'Junior')
