r, c, m = tuple(map(int, input().split()))

grid = [{} for _ in range(c)]

for _ in range(m):
    row, column, speed, direction, size = tuple(map(int, input().split()))
    
    grid[column][row] = (size, speed, direction)

for man in range(c):
    row = min(grid[man].keys())
    del grid[man][row]

    new_grid = [{} for _ in range(c)]

    for column in range(c):
        for column, shark in grid[column].items():
