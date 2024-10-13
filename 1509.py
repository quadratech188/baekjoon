string = input()

cache = {}

def divide(start, end):
    if (start, end) in cache:
        return cache[(start, end)]
    
    min_ = end - start + 1

    for new_end in range(start, end + 1):
        i = 0
        while i + start < new_end - i:
            if string[i + start] != string[new_end - i]:
                break

        else:
            min_ = min(min_, divide(new_end + 1, end) + 1)

    return min_

divide(0, len(string) - 1)
