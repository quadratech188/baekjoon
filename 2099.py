from typing import List, Optional
import copy

class Matrix:
    @classmethod
    def identity(cls, n: int, MOD: Optional[int] = None) -> 'Matrix':
        return cls([[1 if i == j else 0 for i in range(n)] for j in range(n)], MOD)

    @classmethod
    def zeros(cls, m: int, n: int, MOD: Optional[int] = None) -> 'Matrix':
        return cls([[0 for _ in range(m)] for _ in range(n)], MOD)

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
                for depth in range(n_x):
                    if self.values[y][depth] > 0 and other.values[depth][x] > 0:
                        result[y].append(1)
                        break

                else:
                    result[y].append(0)

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

import sys

n, k, m = tuple(map(int, sys.stdin.readine().split()))

connections = Matrix.zeros(n, n)

for i in range(n):
    a, b = tuple(map(int, sys.stdin.readline().split()))

    connections.values[i][a - 1] = 1
    connections.values[i][b - 1] = 1

power = connections ** k

for _ in range(m):
    a, b = tuple(map(int, sys.stdin.readline().split()))

    query = Matrix.zeros(n, 1)
    query.values[0][a - 1] = 1

    if (query * power).values[0][b - 1] > 0:
        print('death')
    else:
        print('life')


