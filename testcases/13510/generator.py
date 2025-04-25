import random

def generate_random_tree(n):
    """Generates a random tree with n nodes (0 to n-1) using Prüfer sequence."""
    if n <= 1:
        return []

    # Step 1: Generate a random Prüfer sequence of length n - 2
    prufer = [random.randint(0, n - 1) for _ in range(n - 2)]

    # Step 2: Initialize vertex degrees
    degree = [1] * n
    for node in prufer:
        degree[node] += 1

    # Step 3: Generate the tree from the Prüfer sequence
    tree = []
    leaf = 0
    ptr = 0
    while degree[leaf] != 1:
        leaf += 1

    for node in prufer:
        tree.append((leaf, node))
        degree[leaf] -= 1
        degree[node] -= 1
        if degree[node] == 1 and node < ptr:
            leaf = node
        else:
            ptr += 1
            while ptr < n and degree[ptr] != 1:
                ptr += 1
            leaf = ptr

    # Add the last edge
    u, v = [i for i in range(n) if degree[i] == 1]
    tree.append((u, v))
    return tree

# Example usage
import sys
n = int(sys.argv[1])  # number of nodes
print(n)
tree = generate_random_tree(n)
for edge in tree:
    print(edge[0] + 1, edge[1] + 1, random.randint(0, 100000))

m = int(sys.argv[2])
print(m)
for i in range(m):
    tp = random.choice((1, 2))
    if (tp == 1):
        print(tp, random.randint(1, n - 1), random.randint(0, 100000))
    else:
        print(tp, random.randint(1, n), random.randint(1, n))
