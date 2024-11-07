#include "modules/SegmentTree.h"
#include <stdio.h>

struct Data {
	int max_full;
	int max_left;
	int max_right;
	int max_floating;

	Data operator+(const Data& other) const {
		Data result;
		result.max_full = this->max_full + other.max_full;

		result.max_left = std::max(this->max_left, this->max_full + other.max_left);

		result.max_right = std::max(other.max_right, this->max_right + other.max_full);

		result.max_floating = std::max(std::max(this->max_floating, other.max_floating), this->max_right + other.max_left);
		return result;
	}

	void update(int action) {
		this->max_full = action;
		this->max_left = action;
		this->max_right = action;
		this->max_floating = action;
	}
};

int main() {
	int n, q, u, v;
	scanf("%d %d %d %d", &n, &q, &u, &v);

	std::vector<Data> values(n);

	for (auto& value: values) {
		int a;
		scanf("%d", &a);
		value = {
			.max_full = u * a + v,
			.max_left = u * a + v,
			.max_right = u * a + v,
			.max_floating = u * a + v
		};
	}
	
	SegmentTree<Data, int> tree(values);

	for (int i = 0; i < q; i++) {
		int t, b;
		size_t a;
		scanf("%d %zu %d", &t, &a, &b);
		switch(t) {
			case 0:
				printf("%d\n", tree.query({a - 1, b}).max_floating - v);
				break;
			case 1:
				tree.update(a - 1, u * b + v);
		}
	}
}
