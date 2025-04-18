#include "modules/SegTree.cpp"
#include <algorithm>
#include <vector>

#include <stdio.h>

std::vector<int> values;

std::pair<int, int> combine_func(std::pair<int, int> val1, std::pair<int, int> val2) {
	return {
		std::min(val1.first, val2.first),
		std::max(val1.second, val2.second)
	};
}

std::pair<int, int> value_func(int index) {
	return {values[index], values[index]};
}

int main() {

	int n, q;

	scanf("%d %d", &n, &q);

	values = std::vector<int>(n);

	for (int i = 0; i < n; i++) {
		scanf("%d", &values[i]);
	}
	
	auto root = SegmentNode<int, std::pair<int, int>>({0, n}, combine_func, value_func);

	for (int i = 0; i < q; i++) {
		int a, b;
		scanf("%d %d", &a, &b);

		std::pair<int, int> minMax = root.query({a - 1, b});

		printf("%d\n", minMax.second - minMax.first);
	}

	return 0;
}
