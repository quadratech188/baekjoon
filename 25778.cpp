#include "modules/SegTree.cpp"
#include <vector>
#include <cstdio>

std::vector<long long int> values;

long long int combine_func(long long int val1, long long int val2) {
	return val1 + val2;
}

long long int value_func(int index) {
	return values[index];
}

int main() {
	int n;
	scanf("%d", &n);

	values = std::vector<long long int>(n, 0);

	for (int i = 0; i < n; i++) {
		scanf("%lld", &values[i]);
	}

	int t;
	scanf("%d", &t);

	auto root = SegmentNode<int, long long int>({0, n}, combine_func, value_func);

	for (int i = 0; i < t; i++) {
		char x;
		int y, z;
		scanf(" %c %d %d", &x, &y, &z);

		if (x == 'U') {
			values[y - 1] += z;
			root.update(y - 1);
		}
		else if (x == 'R') {
			printf("%lld\n", root.query({y - 1, z}));
		}
	}
}
