def from_secs(x):
    return 3600 * x[0] + 60 * x[1] + x[2]

def to_secs(s):
    return (s // 3600, (s // 60) % 60, s % 60)

for _ in range(3):
    x = list(map(int, input().split()))

    print(*to_secs(from_secs(x[3:]) - from_secs(x[:3])))
