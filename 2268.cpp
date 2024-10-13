#include "modules/SegTree.cpp"
#include <vector>
#include <utility>

#include <stdio.h>

std::vector<long long int> values;

long long int combine_func(long long int val1, long long int val2) {
	return val1 + val2;
}

long long int value_func(int index) {
	return values[index];
}

int main() {

	int n, m;

	scanf("%d %d", &n, &m);

	values = std::vector<long long int>(n, 0);
	
	auto root = SegmentNode<int, long long int>({0, n}, combine_func, value_func);

	for (int i = 0; i < m; i++) {
		int a, b, c;
		scanf("%d %d %d", &a, &b, &c);

		if (a == 0) {
			if (b > c) {
				std::swap(b, c);
			}

			long long int sum = root.query({b - 1, c});

			printf("%lld", sum);

		}
		else {
			values[b - 1] = c;
			root.update(b - 1);
		}
	}

	return 0;
}
