N = 16
Q = 6
R = 2

# 1
print(R * N + 1)

# (5R - 2)N + 1
for i in range(N):
    print(R, R * (i + 1), 0, end=' ')
    for k in range(1, R):
        print(R * i + k, k * R ** (N - i), end = ' ')
    print()

    for k in range(1, R):
        print(1, R * (i + 1), - 2 * k * R ** (N - i))
print(0)

# 2Q + 1
print(Q)
for _ in range(Q):
    print(0, R * N)

# Total: 5RN - 2N + 2Q + 3
