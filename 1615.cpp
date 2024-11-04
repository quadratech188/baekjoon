#include "modules/SegmentTree.h"
#include <algorithm>
#include <stdio.h>
#include <utility>

struct Count {
	int value;
	Count operator+(const Count other) const {
		return {
			.value = this->value + other.value
		};
	}
	void update(int action) {
		this->value += action;
	}
};

int main() {
	int n, m;
	scanf("%d %d", &n, &m);
	SegmentTree<Count, int> data(n, {0});

	std::vector<std::vector<int>>values(m);

	for (int i = 0; i < m; i++) {
		int a, b;
		scanf("%d %d", &a, &b);
		values[a - 1].push_back(b - 1);
	}

	long long int sum = 0;

	for (auto& start: values) {
		for (auto& end: start) {
			if (end < n - 1)
				sum += data.query({end + 1, n}).value;
		}
		for (auto& end: start) {
			data.update(end, 1);
		}
	}
	printf("%lld\n", sum);
}
