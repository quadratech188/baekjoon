#include <iostream>
#include <limits>
#include "modules/FastIO.h"
#include "modules/SegmentTree.h"

template <typename T>
struct Min {
	T value;
	Min operator+(const Min& other) const {
		return {
			.value = std::max(this->value, other.value)
		};
	}
};

int main() {
	FastIO();
	while (true) {
		int n;
		std::cin >> n;
		
		if (n == 0) break;

		std::vector<int> values(2 * n);
		std::vector<Min<int>> minValues(2 * n);

		for (int i = 0; i < n; i++) {
			std::cin >> values[i];
			values[i + n] = values[i];
			minValues[i] = {values[i]};
			minValues[i + n] = {values[i + n]};
		}

		SegmentTree<int, bool> sumSeg(values);
		SegmentTree<Min<int>, bool> maxSeg(minValues);

		int min = std::numeric_limits<int>::max();

		for (int i = 0; i < n; i++) {
			min = std::min(min, sumSeg.query({i, n / 2 + i}) - maxSeg.query({i, n / 2 + i}).value);
		}

		std::cout << (min + sumSeg.root() / 2) << std::endl;
	}
}
