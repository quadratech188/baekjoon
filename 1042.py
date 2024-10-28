code = input()
n = int(input())

proteins = set()

for _ in range(n):
    code, _ = input().split()
    proteins.add(code)

def solution(head: int):
    if head > len(code) - 3:
        return 0
    result = 0
    if code[head:head+3] in proteins:
        result += solution(head + 3)

    return result

