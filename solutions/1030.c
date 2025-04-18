#include <stdio.h>

int isInMiddle(int n, int k, int x, int y) {
	return (n-k)/2 <= x && x < (n+k)/2 && (n-k)/2 <= y && y < (n+k) /2;
}

int isInFractal(int s, int n, int k, int x, int y) {
	while(s != 0) {
		if (isInMiddle(n, k, x % n, y % n)) {
			return 1;
		}
		x /= n;
		y /= n;
		s--;
	}
	return 0;
}

int main() {
	int s, N, K, R1, R2, C1, C2;

	scanf("%d %d %d %d %d %d %d", &s, &N, &K, &R1, &R2, &C1, &C2);

	for (int x = R1; x <= R2; x++) {
		for (int y = C1; y <= C2; y++) {
			printf("%d", isInFractal(s, N, K, x, y));
		}
		printf("\n");
	}
}
