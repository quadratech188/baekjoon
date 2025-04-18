class Matrix:
    def __init__(self, n):
        self.n = n

    def get_numbers_before_or_equal_to(self, target) -> int:
        total = 0
        for i in range(1, self.n + 1):
            total += min(target // i, self.n)

        return total

matrix = Matrix(int(input()))


k = int(input())

range_ = (0, k)

while True:
    if range_[1] - range_[0] == 1:
        print(range_[1])
        break
    center = int((range_[0] + range_[1]) / 2)
    center_value = matrix.get_numbers_before_or_equal_to(center)

    if k <= center_value:
        range_ = (range_[0], center)
    else:
        range_ = (center, range_[1])
