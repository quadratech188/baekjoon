
def set_child(graph, node, new_adj, visited):
    visited.add(node)
    for child in graph[node]:
        if child in visited:
            continue
        new_adj[child] = node
        set_child(graph, child, new_adj, visited)

def main():
    n = int(input())

    adjacency = {i: [] for i in range(n)}

    for _ in range(n - 1):
        a, b = tuple(map(int, input().split()))
        adjacency[b - 1].append(a - 1)
        adjacency[a - 1].append(b - 1)

    new_adj = {i: None for i in range(n)}
    
    set_child(adjacency, 0, new_adj, set())

    adjacency = new_adj
    p = int(input())
    for _ in range(p):
        a, b = tuple(map(int, input().split()))
        a -= 1
        b -= 1
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

        cont = 0
        for i, j in zip(reversed(a_parents), reversed(b_parents)):
            if i != j: break
            cont += 1

        print(a_parents[-cont] + 1)
main()
