#include <iostream>
#include <vector>

int cache[1001][1001];

int main() {
	int n;
	std::cin >> n;

	for (int i = 0; i < n * n; i++) {
		int garbage;
		std::cin >> garbage;
	}

	cache[n][n] = 1;
	for (int i = n - 1; i >= 0; i--) {
		cache[i][n] = 1;
	}
	for (int j = n - 1; j >= 0; j--) {
		cache[n][j] = 1;
	}

	for (int i = n - 1; i >= 0; i--) {
		for (int j = n - 1; j >= 0; j--) {
			cache[i][j] = cache[i + 1][j] + cache[i][j + 1];
		}
	}

	std::cout << cache[0][0] << ' ' << n * n;
}
