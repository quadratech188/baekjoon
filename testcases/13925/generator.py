import random
import sys

MOD = 10**9 + 7

def generate_testcase(N, M, seed=42):
    random.seed(seed)

    A = [random.randint(1, 10**9) for _ in range(N)]

    print(N)
    print(" ".join(map(str, A)))
    print(M)

    for _ in range(M):
        query_type = random.randint(1, 4)
        x = random.randint(1, N)
        y = random.randint(x, N)  # ensure x ≤ y

        if query_type in {1, 2, 3}:
            v = random.randint(1, 10**9)
            print(f"{query_type} {x} {y} {v}")
        else:  # query_type == 4
            print(f"4 {x} {y}")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python3 generate_testcase.py <N> <M>")
        sys.exit(1)

    N = int(sys.argv[1])
    M = int(sys.argv[2])

    generate_testcase(N, M)

