n = int(input())
locations = list(map(int, input().split()))
times = list(map(int, input().split()))

print(max(2 * max(locations), max((l + t for l, t in zip(locations, times)))))
