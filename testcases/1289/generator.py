import sys
import random

def generate_tree(n, weight_range=(0, 1000), seed=None):
    if seed is not None:
        random.seed(seed)

    edges = []
    nodes = list(range(1, n + 1))
    random.shuffle(nodes)

    for i in range(1, n):
        u = nodes[i]
        v = nodes[random.randint(0, i - 1)]
        w = random.randint(*weight_range)
        edges.append((u, v, w))

    return n, edges

def print_tree(n, edges):
    print(n)
    for u, v, w in edges:
        print(f"{u} {v} {w}")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python3 gen.py <n> [seed]", file=sys.stderr)
        sys.exit(1)

    n = int(sys.argv[1])
    seed = int(sys.argv[2]) if len(sys.argv) >= 3 else None

    n, edges = generate_tree(n, seed=seed)
    print_tree(n, edges)

