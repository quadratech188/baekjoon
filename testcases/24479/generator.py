import sys
import random

def generate_testcase(n, m, r):
    max_edges = n * (n - 1) // 2
    m = min(m, max_edges)  # 최대 간선 수 초과 방지

    # 간선 생성: u < v 조건을 만족하는 (u, v) 쌍을 무작위로 선택
    edges = set()
    while len(edges) < m:
        u = random.randint(1, n)
        v = random.randint(1, n)
        if u == v:
            continue
        if u > v:
            u, v = v, u
        edges.add((u, v))

    edges = list(edges)
    random.shuffle(edges)

    # 출력
    print(n, m, r)
    for u, v in edges:
        print(u, v)

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print(f"Usage: {sys.argv[0]} N M R", file=sys.stderr)
        sys.exit(1)

    n = int(sys.argv[1])
    m = int(sys.argv[2])
    r = int(sys.argv[3])
    generate_testcase(n, m, r)

