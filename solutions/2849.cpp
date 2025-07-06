#include "../modules/SegmentTree2.h"
#include "../modules/FastIO.h"
#include "../modules/FastIO2.h"
#include <algorithm>

enum Type {
	L,
	R
};

struct Element {

	Type left;
	Type right;

	uint _max_, _max, max_;

	Element():
		left(L), right(L), _max_(1), _max(1), max_(1) {}

	void reinit(Element const& l, Element const& r) {
		left = l.left;
		right = r.right;

		if (l.right == r.left) {
			_max_ = std::max(l._max_, r._max_);
			_max = r._max;
			max_ = l.max_;
		}
		else {
			_max_ = std::max({l._max_, l._max + r.max_, r._max_});
			
			if (r.max_ == SegmentTree<Element>::combine_ctx.right.size())
				_max = l._max + SegmentTree<Element>::combine_ctx.right.size();
			else
			 	_max = r._max;

			if (l.max_ == SegmentTree<Element>::combine_ctx.left.size())
				max_ = SegmentTree<Element>::combine_ctx.left.size() + r.max_;
			else
			 	max_ = l.max_;
		}
	}

	Element operator+(Element const& other) {
		Element result;
		result.reinit(*this, other);
		return result;
	}

	void toggle() {
		if (left == L)
			left = right = R;
		else
		 	left = right = L;
	}
};

int main() {
	FastIO();
	uint N, Q;
	Fast::cin >> N >> Q;

	SegmentTree<Element> tree(N);

	for (uint i = 0; i < Q; i++) {
		uint index;
		Fast::cin >> index;

		tree.update(index - 1, &Element::toggle);

		std::cout << tree.root()._max_ << '\n';
	}
}
