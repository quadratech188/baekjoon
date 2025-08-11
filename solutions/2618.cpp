#include "FastIO2.h"
#include "Vec2.h"
#include "Matrix.h"
#include <iostream>

std::vector<Int2> cases;

Matrix<int> cache;
Matrix<char> which;

int solve(uint i1, uint i2) {
	int& result = cache(i1, i2);

	if (result != -1)
		return result;

	uint next = std::max(i1, i2) + 1;

	if (next == cases.size()) return 0;

	int candidate1 = cases[i1].taxi_distance(cases[next]) + solve(next, i2);
	int candidate2 = cases[i2].taxi_distance(cases[next]) + solve(i1, next);

	if (candidate1 < candidate2) {
		which(i1, i2) = 1;
		result = candidate1;
		return candidate1;
	}
	else {
		which(i1, i2) = 2;
		result = candidate2;
		return candidate2;
	}
}

int main() {
	int n;
	uint w;
	Fast::cin >> n >> w;

	cases.resize(w + 2);
	cases[0] = {1, 1};
	cases[1] = {n, n};

	for (uint i = 2; i < w + 2; i++)
		Fast::cin >> cases[i];

	which = Matrix<char>(w + 2, w + 2);
	cache = Matrix<int>(w + 2, w + 2, -1);

	std::cout << solve(0, 1) << '\n';

	uint i1 = 0;
	uint i2 = 1;

	while (true) {
		uint next = std::max(i1, i2) + 1;
		if (next == cases.size()) break;
		if (which(i1, i2) == 1) {
			std::cout << "1\n";
			i1 = next;
		}
		else {
			std::cout << "2\n";
			i2 = next;
		}
	}
}
