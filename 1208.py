def main():
    n, s = tuple(map(int, input().split()))

    values = map(int, input().split())

    sum = 0
    sums = []

    for value in values:
        sum += value

        sums.append(sum)
    
    def subsum(a, b):
        if a == 0:
            return sums[b]
        else:
            return sums[b] - sums[a - 1]
