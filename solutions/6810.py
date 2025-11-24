x = list(int(input()) for _ in range(3))
c = x[0] + 3 * x[1] + x[2]

print(f'The 1-3-sum is {c + 120 - 9 - 4 * 3 - 8}')
