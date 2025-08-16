#include "FastIO2.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>

int main() {
	int d, p, q;
	Fast::cin >> d >> p >> q;

	if (p < q)
		std::swap(p, q);

	// iterations = min(q, d / p)
	int min_cost = std::numeric_limits<int>::max();
	for (int i = 0; i < q; i++) {
		int q_count = std::max((int)std::ceil((double)(d - i * p) / q), 0);
		min_cost = std::min(min_cost, i * p + q_count * q);
		if (q_count == 0) break;
	}
	std::cout << min_cost;
}
