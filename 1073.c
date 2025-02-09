#include <stdio.h>

int lookup[10] = {1, 0, 1, 0, 3, 0, 15, 0, 105, 0};
int degree[10] = {0};

int main() {
	int n;
	scanf("%d", &n);


	for (int i = 0; i < 2 * n; i++) {
		char t;
		scanf(" %c", &t);
		degree[t - '0'] ++;
	}

	long long int result = 1;
	for (int i = 0; i < 10; i++)
		result *= lookup[degree[i]];

	printf("%lld", result);
}
