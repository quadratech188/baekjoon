n, k, m = tuple(map(int, input().split()))

pointer = 1

count = 0
while True:
    count += 1

    pointer += k - 1

    pointer = (pointer - 1) % n + 1

    if pointer < m:
        n -= 1
        m -= 1
        continue
    if pointer == m:
        print(count)
        break
    if pointer > m:
        n -= 1
