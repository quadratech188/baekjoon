#include "FastIO2.h"
#include "Vec2.h"
#include <iostream>
#include <map>

int main() {
	uint n;
	Fast::cin >> n;
	std::vector<Int2> points = Fast::cin.to_vec<Int2>(n);

	std::map<int, int64_t> x_counts, y_counts;
	for (Int2 p: points) {
		x_counts[p.x] ++;
		y_counts[p.y] ++;
	}

	int64_t result = 0;
	for (auto p: points) {
		result += (x_counts[p.x] - 1) * (y_counts[p.y] - 1);
	}
	std::cout << result;
}
