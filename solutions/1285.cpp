#include <cmath>
#include <iostream>
#include <limits>

#include "modules/Matrix.h"

int main() {
	int n;
	std::cin >> n;

	Matrix<char> coins(n, n);
	std::cin >> coins;

	int min = std::numeric_limits<int>::max();

	for (int i = 0; i < std::pow(2, n); i++) {
		int sum = 0;

		for (int row = 0; row < n; row++) {
			int tailCount = 0;
			for (int column = 0; column < n; column++) {
				if (i & (1 << column))
					tailCount += coins[column][row] == 'H'? 1: 0;
				else
				 	tailCount += coins[column][row] == 'T';
			}
			sum += std::min(tailCount, n - tailCount);
		}

		min = std::min(min, sum);
	}

	std::cout << min;
}
