from typing import List, Tuple

class Board:
    def __init__(self, size: int, board: List[List[int]]):
        self.size = size
        self.board = board

    def move(direction)

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

