#include "modules/SegmentTree.h"
#include <stdio.h>

int main() {
	int n, m;
	scanf("%d %d", &n, &m);
	SegmentTree<int> data(n);

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
				sum += data.sum(end + 1, n);
		}
		for (auto& end: start) {
			data.update(end, [](int& data) {data++;});
		}
	}
	printf("%lld\n", sum);
}
