#include "modules/SegTree.h"
#include <stdio.h>

struct Min {
	int value;
	size_t index;
	inline Min operator+(const Min other) const {
		if (this->value <= other.value) {
			return *this;
		}
		else {
			return other;
		}
	}
	void update(int action) {
		this->value = action;
	}
};

int main() {
	int n;
	scanf("%d", &n);

	std::vector<Min> values(n);

	for (size_t i = 0; i < n; i++) {
		int value;
		scanf("%d", &value);
		values[i] = {
			.value = value,
			.index = i + 1
		};
	}

	SegmentTree<Min, int> tree(values);

	int m;
	scanf("%d", &m);

	for(int i = 0; i < m; i++) {
		int a, b, c;
		scanf("%d %d %d", &a, &b, &c);

		switch(a) {
			case 1:
				tree.update(b - 1, c);
				break;
			case 2:
				printf("%zu\n", tree.query({b - 1, c}).index);
		}
	}
}
