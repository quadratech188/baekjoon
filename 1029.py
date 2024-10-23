n = int(input())

prices = []

for _ in range(n):
    prices.append(list(map(int, input())))

cache = {}

def max_times(current_artist: int, artist_bitmap: int, bought_for: int):
    if (current_artist, artist_bitmap, bought_for) in cache:
        return cache[(current_artist, artist_bitmap, bought_for)]

    max_val = 1
    for next_artist in range(n):
        if next_artist == current_artist:
            continue
           
        if artist_bitmap & (1 << next_artist) != 0:
            continue

        if prices[current_artist][next_artist] >= bought_for:
            max_val = max(
                    max_val,
                    max_times(next_artist,
                              artist_bitmap | (1 << current_artist),
                              prices[current_artist][next_artist]
                    ) + 1
            )

    cache[(current_artist, artist_bitmap, bought_for)] = max_val
    return max_val

print(max_times(0, 0, 0))
