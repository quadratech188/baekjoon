def main():
    n = int(input())

    adjacency = {i: set() for i in range(n)}

    for _ in range(n - 1):
        a, b = tuple(map(int, input().split()))
        adjacency[b - 1].add(a - 1)
        adjacency[a - 1].add(b - 1)

    a, b = tuple(map(int, input().split()))
    a -= 1
    b -= 1

    root = 0
    
    a_parents = []
    b_parents = []

    while True:
        b_parents.append(b)
        if adjacency[b] == None:
            break
        b = adjacency[b]

    while True:
        a_parents.append(a)
        if adjacency[a] == None:
            break
        a = adjacency[a]

    combined = set(a_parents) & set(b_parents)

    for i in a_parents:
        if i in combined:
            print(i + 1)
            return

for _ in range(int(input())):
    main()
