def number_of_segments(left: int, n: int, right: int):
    return (n - left) * (right - n) - 1
