import copy
n = int(input())

board = []

for _ in range(n):
    board.append(list(map(lambda val: val == '1', input().split())))

def solution(board):
    max_ = 0
    for a in range(n):
        for b in range(n):
            if board[a][b] == False: continue
            board_copy = copy.deepcopy(board)

            for x in range(n):
                if 0 <= x - a + b < n:
                    board_copy[x][x - a + b] = False
                if 0 <= a + b - x < n:
                    board_copy[x][a + b - x] = False

            max_ = max(max_, solution(board_copy) + 1)

    return max_

print(solution(board))
