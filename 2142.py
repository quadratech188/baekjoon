def main():
    n, m = tuple(map(int, input().split()))
    last_row = list(map(int, input().split()))

    result = True

    for _ in range(n - 1):
        current_row = list(map(int, input().split()))
        temp = current_row[0] - last_row[0]
        for i in range(1, m):
            temp2 = current_row[i] - last_row[i]
            if temp2 > temp:
                result = False
            temp = temp2

        last_row = current_row

    return result


t = int(input())
for _ in range(t):
    if main():
        print('YES')
    else:
        print('NO')
