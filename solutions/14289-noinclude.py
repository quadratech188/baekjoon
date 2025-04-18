from typing import List, Optional
import copy

class Matrix:
    @classmethod
    def identity(cls, n: int, MOD: Optional[int] = None) -> 'Matrix':
        return cls([[1 if i == j else 0 for i in range(n)] for j in range(n)], MOD)

    def __init__(self, values: List[List[int]], MOD: Optional[int] = None):
        self.values = values
        self.MOD = MOD

    def __mul__(self: 'Matrix', other: 'Matrix'):

        n_x = len(self.values[0])
        n_y = len(self.values)
        m_x = len(other.values[0])
        m_y = len(other.values)

        result = []
        
        for y in range(n_y):
            result.append([])
            for x in range(m_x):
                temp = 0
                for depth in range(n_x):
                    temp += self.values[y][depth] * other.values[depth][x]
                    if self.MOD is not None:
                        temp = temp % self.MOD

                result[y].append(temp)

        return Matrix(result, self.MOD)

    def __pow__(self: 'Matrix', power: int) -> 'Matrix':
        if power == 0:
            return Matrix.identity(len(self.values), self.MOD)

        if power == 1:
                return Matrix(copy.deepcopy(self.values))

        cache = {}
        
        cache[0] = Matrix.identity(len(self.values), self.MOD)
        cache[1] = self
        
        if power % 2 == 0:
            result = cache[0]
        else:
            result = cache[1]
            power -= 1

        power_of_2 = 2

        while power != 0:
            cache[power_of_2] = cache[power_of_2 // 2] * cache[power_of_2 // 2]

            if power % (2 * power_of_2) != 0:
                power -= power_of_2
                result = result * cache[power_of_2]
                
            power_of_2 *= 2
        return result


MOD = 1_000_000_007

n, m = tuple(map(int, input().split()))

mult = Matrix([[0 for _ in range(n)] for _ in range(n)], MOD)

for _ in range(m):
    a, b = tuple(map(int, input().split()))

    mult.values[a-1][b-1] = 1
    mult.values[b-1][a-1] = 1

D = int(input())


initial = Matrix([[0] * n], MOD)
initial.values[0][0] = 1

print((initial * (mult ** D)).values[0][0])
