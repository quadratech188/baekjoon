n, m = tuple(map(int, input().split()))

maze = []

player = (0, 0)

for i in range(n):
    row = input()

    try:
        player_index = row.index('0')
    except:
        pass
    else:
        player = (i, player_index)
        temp = list(row)
        temp[player_index] = '.'
        row = str(temp)
    maze.append(row)

def solution(pos, keys):
    if maze[pos[0]]
