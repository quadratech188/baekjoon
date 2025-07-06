#include <algorithm>
#include <limits>
#include "../modules/SegmentTree2.h"
#include "../modules/FastIO2.h"
#include "../modules/FastIO.h"
#include "../modules/InputRange.h"

struct Element {
	int max;
	int min;
	int b_a;
	int b_c;
	int b_a_c;

	Element() {}

	Element(int value):
		max(value), min(value), 
		b_a(std::numeric_limits<int>::min() / 2),
		b_c(std::numeric_limits<int>::min() / 2),
		b_a_c(std::numeric_limits<int>::min() / 2) {}

	void reinit(Element const& l, Element const& r) {
		b_a = std::max({l.b_a, r.max - l.min, r.b_a});
		b_c = std::max({l.b_c, l.max - r.min, r.b_c});
		/*
		 * a b c |
		 * a b | c
		 * a | b c
		 * | a b c
		 */
		b_a_c = std::max({l.b_a_c, l.b_a - r.min, r.b_c - l.min, r.b_a_c});
		max = std::max(l.max, r.max);
		min = std::min(l.min, r.min);
	}

	Element operator+(Element const& other) {
		Element result;
		result.reinit(*this, other);
		return result;
	}
};

int main() {
	FastIO();
	uint N, Q;
	Fast::cin >> N >> Q;

	SegmentTree<Element> tree(InputRange<int, Fast::istream>(N, Fast::cin));

	for (uint i = 0; i < Q; i++) {
		char type;
		Fast::cin >> type;

		switch(type) {
			case '1': {
				uint x;
				int y;
				Fast::cin >> x >> y;

				tree.update(x - 1, [y](Element& val) {
						val.min = y;
						val.max = y;
						});
				break;
		  	}
			case '2': {
				uint l, r;
				Fast::cin >> l >> r;
				std::cout << tree.sum(l - 1, r).b_a_c << '\n';
		  	}
		}
	}
}
