n = int(input())

def to_value(char: str):
    if char.isdigit():
        return ord(char) - ord('0')
    return ord(char) - ord('A') + 10

def to_char(index: int):
    if index < 10:
        return str(index)
    return chr(ord('A') + index - 10)

weights = [0] * 36

for i in range(n):
    string = input()

    for index, char in enumerate(string):
        weights[to_value(char)] += 36 ** (len(string) - 1 - index)

k = int(input())

import heapq
large_indices = set(heapq.nlargest(k, range(len(weights)), lambda i: weights[i] * (35 - i)))

total = 0
for index, weight in enumerate(weights):
    total += weight * (35 if index in large_indices else index)

total_str = ''

while total != 0:
    total_str = to_char(total % 36) + total_str
    total = total // 36

print(total_str if total_str != '' else 0)
