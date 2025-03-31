/*
 * Author : thdtjdals3
 * Date : 2021-12-17
 * Description : BOJ 12987
 * Link : https://www.acmicpc.net/problem/12987
 */

#include <stdio.h>

typedef struct {
	int array[50][50];
} Matrix;

Matrix matrix_multiply_modular (Matrix A, Matrix B, int size, int M);
Matrix matrix_power_modular (Matrix Base, int size, int power, int M);
Matrix matrix_power_sum_modular (Matrix Base, int size, int power, int M);

int main() {

	int N, K, M;
	scanf("%d %d %d", &N, &K, &M);
	
	Matrix Base = { {{0}} };
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			scanf("%d", &Base.array[i][j]);
			Base.array[i][j] = ((Base.array[i][j] % M) + M) % M;
		}
	}
	
	Base = matrix_power_sum_modular (Base, N, K, M);
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			printf("%d ", (Base.array[i][j] + M) % M);
		}
		printf("\n");
	}
	
	return 0;
}

Matrix matrix_multiply_modular (Matrix A, Matrix B, int size, int M) {
	Matrix result = { {{0}} };
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			for (int k = 0; k < size; k++)
			{
				result.array[i][j] += A.array[i][k] * B.array[k][j];
			}
            result.array[i][j] %= M;
		}
	}
	return result;
}

Matrix matrix_power_modular (Matrix Base, int size, int power, int M) {
	Matrix result = { {{0}} };
	for (int i = 0; i < size; i++)
	{
		result.array[i][i] = 1;
	}
	while(power > 0)
	{
		if (power % 2)
		{
			result = matrix_multiply_modular (result, Base, size, M);
		}
		Base = matrix_multiply_modular (Base, Base, size, M);
		power /= 2;
	}
	return result;
}

Matrix matrix_power_sum_modular (Matrix Base, int size, int power, int M) {
	if (power == 1)
	{
		return Base;
	}
	else if (power % 2)
	{
		Matrix result = matrix_power_modular (Base, size, power, M);
		Matrix temp = matrix_power_sum_modular (Base, size, power - 1, M);
		for (int j = 0; j < size; j++)
		{
			for (int i = 0; i < size; i++)
			{
				result.array[i][j] = (result.array[i][j] + temp.array[i][j]) % M;
			}
		}
		return result;
	}
	else
	{
		Matrix result = matrix_power_modular (Base, size, power/2, M);
		Matrix temp = matrix_power_sum_modular (Base, size, power/2, M);
		for (int i = 0; i < size; i++)
		{
			result.array[i][i] = (result.array[i][i] + 1) % M;
		}
		result = matrix_multiply_modular (temp, result, size, M);
		return result;
	}
}
