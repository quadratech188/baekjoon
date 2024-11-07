class Board:
    def __init__(self, l: int):
        self.l = l
        self.board= [[False] * l for _ in range(l)]

class Block:
    def __init__(self):
        self.y, self.x = tuple(map(int, input().split()))

        self.shape = []

        for _ in range(self.y):
            self.shape.append(input())

n = int(input())

blocks = []
blocks.append(Block())

def solution(currentIndex: int, currentBoard: Board):
    for offset_x in range(currentBoard.l - blocks[currentIndex].x):
        for offset_y in range(currentBoard.l - blocks[currentIndex].y):
            for row in range(blocks[currentIndex].y):
                if any(currentBoard.board[row + offset_y][i + offset_x] and blocks[currentIndex].shape[row][i] for i in range(blocks[currentIndex].x)):
                    continue

                solution
            

