import copy

class Position:
    def __init__(self, board, available_dominoes):
        self.board = board
        self.available_dominoes = available_dominoes
        
        self.possibilities = []

        for i in range(9):
            for j in range(8):
                if self.board[i][j] == 0 and self.board[i][j + 1] == 0:
                    for domino in available_dominoes:
                        self.possibilities.append((
                            (i, j),
                            'h',
                            domino
                        ))

        for i in range(8):
            for j in range(9):
                if self.board[i][j] == 0 and self.board[i + 1][j] == 0:
                    for domino in available_dominoes:
                        self.possibilities.append((
                            (i, j),
                            'v',
                            domino
                        ))
    
    def apply(self, possibility):
        domino = possibility[2]
        i, j = possibility[0]

        if possibility[1] == 'h':
            self.board[i][j] = domino[0]
            self.board[i][j + 1] = domino[1]

        else:
            self.board[i][j] = domino[0]
            self.board[i + 1][j] = domino[1]
        
        self.available_dominoes.remove(domino)

    def print_board(self):
        for i in range(9):
            for j in range(9):
                print(self.board[i][j], end='')
            print()


def backtrack(position: Position):
    if len(position.available_dominoes) == 0:
        return position

    for possibility in position.possibilities:
        available_dominoes = copy.copy(position.available_dominoes)
        board = copy.deepcopy(position.board)
        domino = possibility[2]
        i, j = possibility[0]

        if possibility[1] == 'h':
            board[i][j] = domino[0]
            board[i][j + 1] = domino[1]

        else:
            board[i][j] = domino[0]
            board[i + 1][j] = domino[1]
        
        position_copy = Position(board, available_dominoes)

        result = backtrack(position_copy)
        if result != None:
            return result

    return None

def coord_to_tuple(pos: str):
    index = {
        'A': 0,
        'B': 1,
        'C': 2,
        'D': 3,
        'E': 4,
        'F': 5,
        'G': 6,
        'H': 7,
        'I': 8
    }

    return (
        index[pos[0]],
        int(pos[1]) - 1
    )

def loop():
    n = int(input())
    if n == 0:
        raise StopIteration
    
    board = []
    
    for _ in range(9):
        board.append([0] * 9)

    available_dominoes = set([(j, i) for i in range(1, 10) for j in range(i)]
)
    for _ in range(n):
        val1, pos1, val2, pos2 = input().split()

        val1 = int(val1)
        val2 = int(val2)
        pos1 = coord_to_tuple(pos1)
        pos2 = coord_to_tuple(pos2)

        if val1 < val2:
            available_dominoes.remove((val1, val2))
        else:
            available_dominoes.remove((val2, val1))

        x1, y1 = pos1
        x2, y2 = pos2
        board[x1][y1] = val1
        board[x2][y2] = val2

    numbers = input().split()

    for index, k in enumerate(numbers):
        x, y = coord_to_tuple(k)
        board[x][y] = index + 1

    print(board)
    print(available_dominoes)

    backtrack(Position(board, available_dominoes)).print_board()

while True:
    try:
        loop()
    except StopIteration:
        break
    
