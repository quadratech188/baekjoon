from typing import List, Optional

def solution(last: int, safe_until: int, sum: int) -> Optional[List[int]]:
    if sum == 0:
        return []

    for next in range(safe_until, last, -1):
        if next > sum:
            continue

        t = solution(next, safe_until + next, sum - next)

        if t is not None:
            return [next] + t
        
        next += 1

    return None

n = int(input())
sol = solution(0, 1, n)

if sol == None:
    print('-1')
else:
    letters = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z']
    answer = []
    letter_index = 0
    for index in sol:
        answer.insert(index - 1, letters[letter_index])
        for _ in range(index - 1):
            answer.append(letters[letter_index])
        letter_index += 1

    print(*answer, sep = '')

