#include <iostream>
#include <limits>

int table[100'000][20];

int main() {
	int n, m;
	std::cin >> n >> m;

	for (int i = 0; i < n; i ++) {
		std::cin >> table[i][0];
	}

	for (int j = 1; ; j ++) {
		int range_len = 1 << j;
		if (range_len > n) break;

		for (int i = 0; i <= n - range_len; i++) {
			// [i, i + range_len - 1]

			table[i][j] = std::min(table[i][j - 1], table[i + range_len / 2][j - 1]);
		}
	}

	for (int _ = 0; _ < m; _ ++) {
		int a, b;
		std::cin >> a >> b;
		a --;
		b --;
		int range_len = b - a + 1;

		int result = std::numeric_limits<int>::max();

		int ptr = a;
		for (int j = 0; range_len != 0; j++) {
			if (range_len & (1 << j)) {
				result = std::min(result, table[ptr][j]);
				ptr += 1 << j;
				range_len -= 1 << j;
			}
		}
		std::cout << result << '\n';
	}
}
