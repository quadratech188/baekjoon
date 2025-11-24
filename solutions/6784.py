n = int(input())

responses = list(input() for _ in range(n))
answers = list(input() for _ in range(n))

print(sum(a == b for a, b in zip(responses, answers)))
