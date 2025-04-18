#include "modules/Matrix.h"
#include <iostream>

int sum(Matrix<int>& sum, Int2 left, Int2 right) {
	return sum[right.x + 1][right.y + 1] - sum[right.x + 1][left.y + 1]
		- sum[left.x + 1][right.y + 1] + sum[left.x + 1][left.y + 1];
}

int main() {
	int n, m, x, y;
	std::cin >> n >> m >> x >> y;
	Matrix<int> people(m + 1, n + 1);
	std::cin >> people;

	Matrix<int> sum(m + 2, n + 2);
	sum[0][0] = 0;

	for (int x = 1; x < sum.size().x; x++) {
		sum[x][0] = sum[x - 1][0] + people[x - 1][0];
	}
	for (int y = 1; y < sum.size().x; y++) {
		sum[0][y] = sum[0][y - 1] + people[0][y - 1];
	}

	for (int x = 1; x < sum.size().x; x++) {
		for (int y = 1; y < sum.size().y; y++) {
			sum[x][y] = people[x - 1][y - 1] + sum[x - 1][y] + sum[x][y - 1] - sum[x - 1][y - 1];
		}
	}

	// Always use 0, n + 1
	
	std::vector<int> partitions(x + 2);
	partitions[0] = 0;
	partitions[x + 1] = n;

	for (int i = 0; i < (1 << n); i++) {
		int cnt = 0;
		for (int idx = 0; idx < n; idx++) {
			cnt++;
			partitions[cnt] = idx + 1;
			if (cnt == x) break;
		}
	}
}
