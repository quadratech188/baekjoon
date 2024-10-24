def longest(arr):
    if len(arr) == 1:
        return 1

    maxlen = 0

    for start in range(len(arr)):
        for end in range(start + 1, len(arr)):
            if not (arr[end] > arr[end - 1]):
                if maxlen < end - start:
                    maxlen = end - start

                break
    
    return maxlen

arr = list(map(int, input().split()))

print(longest(arr))
