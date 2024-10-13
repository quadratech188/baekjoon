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

void loop() {
	int b, p, q;
	scanf("%d %d %d", &b, &p, &q);

	values = std::vector<int>(b, 0);

	auto root = SegmentNode<int, int>({0, b}, combine_func, value_func);

	for (int i = 0; i < p + q; i++) {
		char x;
		int y, z;
		scanf(" %c %d %d", &x, &y, &z);

		if (x == 'P') {
			values[y - 1] += z;
			root.update(y - 1);
		}
		else if (x == 'Q') {
			printf("%d\n", root.query({y - 1, z}));
		}
	}
}

int main() {
	int t;
	scanf("%d", &t);
	for (int i = 0; i < t; i++) {
		loop();
	}
}
