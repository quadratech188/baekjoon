n = int(input())

values = []

for _ in range(n):
    values.append(list(map(int, list(input()))))

print(values)

cache = {}

def value(person, max_bought_for):
    if (person, max_bought_for) in cache:
        return cache[(person, max_bought_for)]
    result = 0

    for i in range(n):
        if values[i][person] <= max_bought_for:
            result += value(i, values[i][person])

    return result
