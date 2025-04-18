from typing import List, Tuple
import copy

class Board:
    @classmethod
    def input(cls, rows, columns):
        board = []
        for i in range(rows):
            board.append(list(input()))

        return Board(board)

    def __init__(self, board: List[List[str]]):
        self.board = board
        
        for i, row in enumerate(board):
            for j, square in enumerate(row):
                if square == 'R':
                    self.red_pos = (j, i)

                if square == 'B':
                    self.blue_pos = (j, i)

    def coords(self, pos: Tuple[int, int], replace=None) -> str:
        if pos[0] < 0 or pos[0] >= len(self.board[0]):
            raise IndexError

        if pos[1] < 0 or pos[1] >= len(self.board):
            raise IndexError
        
        if replace:
            self.board[pos[1]][pos[0]] = replace

        return self.board[pos[1]][pos[0]]

    def move_until(self, pos: Tuple[int, int], translate: Tuple[int, int]):
        while True:
            new_pos = (pos[0] + translate[0], pos[1] + translate[1])
            #print(new_pos)
            try:
                value = self.coords(new_pos)
            except IndexError:
                return pos

            if value == 'O':
                raise InterruptedError('Marble went in the hole')
            
            if value != '.':
                return pos

            pos = new_pos

    def move_marbles(self, translate: Tuple[int, int]):
        success = None
        modified = False
        try:
            past_pos = self.red_pos
            self.coords(self.red_pos, '.')
            self.red_pos = self.move_until(self.red_pos, translate)
            self.coords(self.red_pos, 'R')
            if past_pos != self.red_pos:
                modified = True
        except InterruptedError:
            success = True
        try:
            past_pos = self.blue_pos
            self.coords(self.blue_pos, '.')
            self.blue_pos = self.move_until(self.blue_pos, translate)
            self.coords(self.blue_pos, 'B')
            if past_pos != self.blue_pos:
                modified = True
        except InterruptedError:
            success = False
        try:
            past_pos = self.red_pos
            self.coords(self.red_pos, '.')
            self.red_pos = self.move_until(self.red_pos, translate)
            self.coords(self.red_pos, 'R')
            if past_pos != self.red_pos:
                modified = True
        except InterruptedError:
            if success is None: # If the blue marble didn't go in yet
                success = True

        return success, modified

def prettyprint(board: Board):
    for row in board.board:
        print(row)

def main():
    dimensions = input().split()
    initial_board = Board.input(int(dimensions[0]), int(dimensions[1]))
    translates = [
        (0, 1),
        (1, 0),
        (0, -1),
        (-1, 0)
    ]
    queue = []
    queue.append((initial_board, 0))

    while True:
        if len(queue) == 0:
            print('-1')
            return
        board, generation = queue.pop(0)
        if generation >= 10:
            print('-1')
            return;
        #print(f'Generation {generation}')
        #print(len(queue))
        #prettyprint(board)
        
        for translate in translates:
            board_copy = copy.deepcopy(board)
            success, modified = board_copy.move_marbles(translate)
            #prettyprint(board_copy)
            #print(modified)
            if success:
                print(generation + 1)
                return
            if success is not False and modified:
                queue.append((board_copy, generation + 1))

main()
