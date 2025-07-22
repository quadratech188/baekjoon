import sys
import random
import string

def random_site_name(existing, length=6):
    while True:
        name = ''.join(random.choices(string.ascii_uppercase, k=random.randint(1, length)))
        if name not in existing:
            return name

def generate_testcase(n, max_in_deg):
    # Generate unique site names
    site_names = []
    for _ in range(n):
        site_names.append(random_site_name(set(site_names)))

    print(n)
    for target in site_names:
        # Choose up to max_in_deg *distinct* other sites as backlinks
        possible_sources = [s for s in site_names if s != target]
        k = random.randint(0, min(max_in_deg, len(possible_sources)))
        sources = random.sample(possible_sources, k)
        print(target, len(sources), *sources)

    # Choose a random site to query
    print(random.choice(site_names))

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python generate_testcase.py <num_sites> <max_in_degree>")
        sys.exit(1)

    n = int(sys.argv[1])
    d = int(sys.argv[2])

    if not (1 <= n <= 50):
        print("num_sites must be between 1 and 50 inclusive.")
        sys.exit(1)
    if not (0 <= d <= 24):
        print("max_in_degree must be between 0 and 24 inclusive.")
        sys.exit(1)

    generate_testcase(n, d)

