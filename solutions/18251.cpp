#include "FastIO2.h"
#include "SegmentTree2.h"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>

struct Element {
	int64_t _max_, _max, max_, max;

	void reinit(Element const& l, Element const& r) {
		_max_ = std::max({l._max_, r._max_, l._max + r.max_});
		_max = std::max(l._max + r.max, r._max);
		max_ = std::max(l.max_, l.max + r.max_);
		max = l.max + r.max;
	}

	Element(int64_t value = 0):
		_max_(value), _max(value), max_(value), max(value) {}

	Element operator+(Element const& other) const {
		Element result;
		result.reinit(*this, other);
		return result;
	}
};

int main() {
	uint n;
	Fast::cin >> n;
	uint height = std::log2(n + 1);

	auto values = Fast::cin.to_vec<int>(n);

	int64_t max = std::ranges::max(values);
	for (uint i = 0; i < height; i++) {
		SegmentTree<Element> tree(n);

		for (uint j = i; j < height; j++) {
			uint step = 1 << (height - j);
			for (uint index = 0; index < (1u << j); index++) {
				int new_value = values[(1u << j) - 1 + index];
				tree.update(step / 2 - 1 + step * index, [new_value](Element& x) {
						x = Element(new_value);
						});
			}
			int64_t result = tree.root()._max_;
			if (result == 0) continue;
			max = std::max(max, result);
		}
	}
	std::cout << max;
}
