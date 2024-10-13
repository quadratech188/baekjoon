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


d = int(input())

mult = Matrix([
        # 학생회관 진리관 신양관 전산관 정보과학관 미래관 한경직기념관 형남공학관
    [0, 1, 0, 0, 0, 0, 0, 1], # 학생회관
    [1, 0, 1, 0, 0, 0, 1, 0], # 진리관
    [0, 1, 0, 1, 0, 1, 1, 0], # 신양관
    [0, 0, 1, 0, 1, 1, 0, 0], # 전산관
    [0, 0, 0, 1, 0, 1, 0, 0], # 정보과학관
    [0, 0, 1, 1, 1, 0, 1, 0], # 미래관
    [0, 1, 1, 0, 0, 1, 0, 1], # 한경직기념관
    [1, 0, 0, 0, 0, 0, 1, 0] # 형남공학관
], MOD)

initial = Matrix([[0, 0, 0, 0, 1, 0, 0, 0]], MOD)

print((initial * (mult ** d)).values[0][4])
