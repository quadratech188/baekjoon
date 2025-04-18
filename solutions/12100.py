from typing import List, Tuple

class Board:
    def __init__(self, size: int, board: List[List[int]]):
        self.size = size
        self.board = board

    def move(self, direction):
        if direction == (0, 1):
            for column in reversed(range(self.size)):
                top = self.size - 1
                for row in range(self.size):
                    if self.board[row][column] != 0:
                        self.board[top][column] = self.board[row][column]
                        self.board[row][column] = 0
                        top -= 1
        if direction == (1, 0):
            for row in range(self.size):
                top = self.size - 1
                for column in reversed(range(self.size)):
                    if self.board[row][column] != 0:
                        self.board[row][top] = self.board[row][column]
                        self.board[row][column] = 0
                        top -= 1
        if direction == (0, -1):
            for column in range(self.size):
                top = 0
                for row in range(self.size):
                    if self.board[row][column] != 0:
                        self.board[top][column] = self.board[row][column]
                        self.board[row][column] = 0
                        top += 1
        if direction == (-1, 0):
            for row in range(self.size):
                top = 0
                for column in range(self.size):
                    if self.board[row][column] != 0:
                        self.board[row][top] = self.board[row][column]
                        self.board[row][column] = 0
                        top += 1
                

    def get_piece(self, pos: Tuple[int, int]) -> int:
        return self.board[pos[0]][pos[1]]
    
    def set_piece(self, pos: Tuple[int, int], value: int) -> None:
        self.board[pos[0]][pos[1]] = value

    def merge(self, direction):
        lookup = {
            (1, 0): ((0, self.size - 1), (0, self.size)),
            (0, 1): ((0, self.size), (0, self.size - 1)),
            (-1, 0): ((1, self.size), (0, self.size)),
            (0, -1): ((0, self.size), (1, self.size))
        }
        ranges = lookup[direction]
        for i in range(ranges[0][0], ranges[0][1]):
            for j in range(ranges[1][0], ranges[1][1]):
                if self.get_piece((i, j)) == self.get_piece( (i + direction[0], j + direction[1]) ):
                    self.set_piece((i, j), 2 * self.get_piece((i, j)))
    
    def play(self, direction):
        self.move(direction)
        self.merge(direction)
        self.move(direction)


board = [
        [2, 0, 0, 0],
        [2, 2, 0, 0],
        [2, 0, 0, 0],
        [0, 0, 0, 0]
        ]

b = Board(4, board)

print(b.board)

while True:
    a = input()
    if a =='h':
        b.play((-1, 0))
    if a =='j':
        b.play((0, -1))
    if a =='k':
        b.play((0, 1))
    if a =='l':
        b.play((1, 0))

    print(b.board)
