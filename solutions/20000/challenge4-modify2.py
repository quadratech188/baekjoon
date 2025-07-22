file = open('challenge4.out').readlines()

for i, line in enumerate(file):
    print(' ' * (len(file) - 1 - i), end='')
    print(line, end='')
