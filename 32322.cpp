#include "modules/SegTree.cpp"
#include <vector>
#include <cstdio>

std::vector<int> values;

int combine_func(int val1, int val2) {
	return val1 + val2;
}

int value_func(int index) {
	return values[index];
}

int main() {
	int n;
	scanf("%d", &n);

	values = std::vector<int>(n, 1);

	int t;
	scanf("%d", &t);

	auto root = SegmentNode<int, int>({0, n}, combine_func, value_func);

	for (int i = 0; i < t; i++) {
		char x;
		int y, z;
		scanf(" %c", &x);

		if (x == 'R') {
			scanf("%d", &y);
			values[y - 1] = 0;
			root.update(y - 1);
		}
		else if (x == 'A') {
			scanf("%d %d", &y, &z);
			printf("%d\n", root.query({y - 1, z}));
		}
	}
}
