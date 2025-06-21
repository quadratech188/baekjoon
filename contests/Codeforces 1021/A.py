t = int(input())

for i in range(t):
    digits = list(map(int, input()))

    answer = ""

    for i in range(1, 11):
        for k in range(10 - i, 9 + 1):
            if k in digits:
                answer += str(k)
                digits.pop(digits.index(k))
                break
    print(answer)

