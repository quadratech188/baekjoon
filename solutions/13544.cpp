#include "../modules/SegmentTree2.h"
#include "../modules/FastIO2.h"
#include "../modules/FastIO.h"
#include "../modules/InputRange.h"
#include <algorithm>
#include <vector>

struct Element {
	Element() = default;

	Element(uint value):
		values({value}) {}

	std::vector<uint> values;

	Element operator+(Element const& other) const {
		Element result;
		result.values.reserve(values.size() + other.values.size());

		std::ranges::merge(values, other.values, std::back_inserter(result.values));

		return result;
	}

	void reinit(Element const&, Element const&) {}
};

int main() {
	FastIO();

	size_t n;
	Fast::cin >> n;

	SegmentTree<Element> tree(
			InputRange<uint, Fast::istream>(n, Fast::cin)
			);

	uint m;
	Fast::cin >> m;

	uint last_ans = 0;

	for (uint _ = 0; _ < m; _++) {
		size_t i, j;
		uint k;

		Fast::cin >> i >> j >> k;

		i ^= last_ans;
		j ^= last_ans;
		k ^= last_ans;

		last_ans = tree.sum(i - 1, j, [k](Element const& val) -> size_t {
				return val.values.end() - std::upper_bound(val.values.begin(), val.values.end(), k);
				});

		std::cout << last_ans << '\n';
	}
}
