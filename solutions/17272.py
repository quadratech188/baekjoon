import copy

def identity(n):
    return [[1 if i == j else 0 for i in range(n)] for j in range(n)]

def matrix_multiply(n, m):
    n_x = len(n[0])
    n_y = len(n)
    m_x = len(n[0])
    m_y = len(m)

    result = []
    
    for y in range(n_y):
        result.append([])
        for x in range(m_x):
            temp = 0
            for depth in range(n_x):
                temp = (temp + n[y][depth] * m[depth][x]) % 1_000_000_007

            result[y].append(temp)

    return result

'''
         0  0  1
         1  0  0
         0  1  1
a1 a2 a3

'''

def power(matrix, power):
    if power == 1:
        return copy.deepcopy(matrix)

    cache = {}
    
    cache[0] = identity(len(matrix))
    cache[1] = matrix
    
    if power % 2 == 0:
        result = cache[0]
    else:
        result = cache[1]
        power -= 1

    power_of_2 = 2

    while power != 0:
        cache[power_of_2] = matrix_multiply(cache[power_of_2 // 2], cache[power_of_2 // 2])

        if power % (2 * power_of_2) != 0:
            power -= power_of_2
            result = matrix_multiply(result, cache[power_of_2])
            
        power_of_2 *= 2
    return result

def main(n, m) -> int:
    if n < m:
        return 1

    if n == m:
        return 2

    initial_values = [[1] * m]

    initial_values[0][m - 1] = 2

    matrix = [[0 for _ in range(m)] for _ in range(m)]

    for x in range(m - 1):
        matrix[x + 1][x] = 1

    matrix[0][m - 1] = 1
    matrix[m - 1][m - 1] = 1
    
    matrix_power = power(matrix, n - m)

    return matrix_multiply(initial_values, matrix_power)[0][m - 1]

n, m = tuple(map(int, input().split()))

print(main(n, m))
