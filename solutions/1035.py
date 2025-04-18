from typing import Tuple, List
from copy import copy

def possible_moves(pos: Tuple[int, int], board: List[List[str]]):
    candidates = [
        (pos[0], pos[1] + 1),
        (pos[0], pos[1] - 1),
        (pos[0] + 1, pos[1]),
        (pos[0] - 1, pos[1])
    ]
    moves = []

    for candidate in candidates:
        if board[candidate[0]][candidate[1]] != '*'\
            and not pos[0] in range(5)\
            and pos[1] in range(5):
            
            moves.append(candidate)

    return moves

def main():
    board = []
    pieces = []
    for i in range(5):
        row = list(input())
        board.append(row)

        pieces.extend([(i, x) for x in range(5) if row[x] == '*'])

    queue = [{'board': board, 'pieces': pieces, 'moves': 0}]

    while len(queue) > 0:
        state = queue.pop(0)
        for index, piece in enumerate(state['pieces']):
            for move in possible_moves(piece, board):
                new_pieces = copy(state['pieces'])

                new_pieces[index] = move

                board[piece[0]][piece[1]] = '.'
                board[move[0]][move[1]] = '*'

                queue.append({
                    'board': 
                })
