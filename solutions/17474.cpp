#include "../modules/LazySegmentTree.h"
#include "../modules/InputRange.h"
#include "../modules/FastIO2.h"
#include "../modules/FastIO.h"
#include <cstdint>
#include <iostream>
#include <algorithm>
#include <limits>

struct Element: public BasicLazy<Element> {
	int max;
	int max_cnt;
	int max2;
	int64_t sum;

	Element() {}

	Element(long long int value):
		max(value),
		max_cnt(1),
		max2(std::numeric_limits<int>::min()),
		sum(value) {}

	Element(int max, int max_cnt, int max2, int64_t sum):
		max(max),
		max_cnt(max_cnt),
		max2(max2),
		sum(sum) {}

	Element operator+(Element const& other) const {
		if (max < other.max)
			return Element(
					other.max,
					other.max_cnt,
					std::max(max, other.max2),
					sum + other.sum
					);

		else if (max == other.max)
			return Element(
					max,
					max_cnt + other.max_cnt,
					std::max(max2, other.max2),
					sum + other.sum
					);

		else
			return Element(
					max,
					max_cnt,
					std::max(max2, other.max),
					sum + other.sum
					);
	}

	void propagate(Element& l, Element& r) {
		l.update(max);
		r.update(max);
	}

	bool update(int new_max) {
		if (new_max >= max) return false;
		if (new_max < max2) return true;

		sum += static_cast<int64_t>(new_max - max) * max_cnt;
		max = new_max;
		return false;
	}
};

int main() {
	size_t n;
	Fast::cin >> n;

	LazySegmentTree<Element> tree(InputRange<int, Fast::istream>(n, Fast::cin));

	uint m;
	Fast::cin >> m;

	for (uint i = 0; i < m; i++) {
		char type;
		size_t l, r;
		Fast::cin >> type >> l >> r;

		switch(type) {
			case '1':
				int x;
				Fast::cin >> x;
				tree.update(l - 1, r, [x](Element& val) {
						return val.update(x);
						});
				break;
			case '2':
				std::cout << tree.sum(l - 1, r).max << '\n';
				break;
			case '3':
				std::cout << tree.sum(l - 1, r).sum << '\n';
		}
	}
}
