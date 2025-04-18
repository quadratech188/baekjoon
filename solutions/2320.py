n = int(input())

words = []

for _ in range(n):
    words.append(input())

max_words = (1 << n) - 1

cache = {}

def solution(current_word: str, used_words: int):
    if used_words == max_words:
        return len(current_word)

    if (current_word, used_words) in cache:
        return cache[(current_word, used_words)]
    
    max_length = 0

    for index in range(n):
        if used_words & (1 << index) != 0:
            continue
        if current_word[-1] != words[index][0]:
            continue
        try:
            length = solution(words[index], used_words | (1 << index))
        except Exception:
            continue
        else:
            max_length = max(length, max_length)

    cache[(current_word, used_words)] = max_length
    return max_length + len(current_word)


max_ = 0
for index, word in enumerate(words):
    max_ = max(max_, solution(word, 1 << index))

print(max_)
