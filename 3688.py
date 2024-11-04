def distance2(pos1, pos2):
    return (pos1[0] - pos2[0]) ** 2 + (pos1[1] - pos2[1]) ** 2

def main():
    inner = int(input())
    inner_vertices = []
    for _ in range(inner):
        inner_vertices.append(tuple(map(int, input().split())))

    outer = int(input())
    outer_vertices = []
    for _ in range(outer):
        outer_vertices.append(tuple(map(int, input().split())))
    inner_ptr = 0
    outer_ptr = 0

    outer1 = outer[0]
    outer2 = outer[1]

    for inner_ptr in range(0, inner):
        while True:
            if 


times = int(input())

for _ in range(times):
    main()
