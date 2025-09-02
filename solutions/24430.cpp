#include <algorithm>
#include <cstdint>
#include <ios>
#include <iostream>

int64_t values[1001][1001];
int64_t cache[1001][1001];

int main() {
	std::cin.tie(0);
	std::ios_base::sync_with_stdio(false);
	int n;
	std::cin >> n;
	
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			std::cin >> values[i][j];
			values[i][j] *= (1ll << 32);
		}
	}

	int p;
	std::cin >> p;
	for (int i = 0; i < p; i++) {
		int x, y;
		std::cin >> x >> y;
		values[x][y] += 1;
	}

	for (int i = 0; i <= n; i++){
		cache[i][0] = 0;
	}
	for (int j = 1; j <= n; j++) {
		cache[0][j] = 0;
	}

	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			cache[i][j] = values[i][j] + std::max(
					cache[i - 1][j],
					cache[i][j - 1]
					);
		}
	}

	int64_t result = cache[n][n];

	std::cout << (result / (1ll << 32)) << ' ' <<(result % (1ll << 32));
}
