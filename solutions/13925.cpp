#include "../modules/FastIO.h"
#include "../modules/FastIO2.h"
#include "../modules/InputRange.h"
#include "../modules/LazySegmentTree.h"
#include "../modules/ModInt.h"
#include <cstdint>
#include <iostream>
#include <ostream>
#include <ranges>

struct Data {
	sm32_1e9_7 a;
	sm32_1e9_7 b;
	sm32_1e9_7 sum;
	int32_t length;

	Data() {}
	Data(sm32_1e9_7 value, int32_t length = 1):
		a(1), b(0), sum(value), length(length) {}

	void update(sm32_1e9_7 c, sm32_1e9_7 d) noexcept {
		a *= c;
		b.mul_add(c, d);
	}
	
	sm32_1e9_7 extract() const noexcept {
		return a * sum + b * sm32_1e9_7::verified(length);
	}

	Data operator+(const Data& other) const noexcept {
		return Data(
				extract() + other.extract(),
				length + other.length
				);
	}

	void propagate(Data& child1, Data& child2) const noexcept {
		child1.update(a, b);
		child2.update(a, b);
	}

	void reinit(Data const& child1, Data const& child2) noexcept {
		sum = child1.extract() + child2.extract();
		a = 1;
		b = 0;
	}

	void apply() noexcept {
		sum = extract();
		a = 1;
		b = 0;
	}
};

int main() {
	FastIO();
	size_t n;
	Fast::cin >> n;

	LazySegmentTree<Data> tree(InputRange<uint, Fast::istream>(n, Fast::cin)
			| std::views::transform([](uint const& val) {return sm32_1e9_7::verified(val);}));

	uint m;
	Fast::cin >> m;

	for (uint i = 0; i < m; i++) {
		char type;
		size_t x, y;
		Fast::cin >> type >> x >> y;

		uint v;
		if (type != '4')
			Fast::cin >> v;

		switch(type) {
			case '1':
				tree.update(x - 1, y, [v](Data& val) {
						val.b += sm32_1e9_7::verified(v);
						});
				break;
			case '2':
				tree.update(x - 1, y, [v](Data& val) {
						val.a *= sm32_1e9_7::verified(v);
						val.b *= sm32_1e9_7::verified(v);
						});
				break;
			case '3':
				tree.update(x - 1, y, [v](Data& val) {
						val.a = 0;
						val.b = sm32_1e9_7::verified(v);
						});
				break;
			case '4':
				std::cout << tree.sum(x - 1, y, [](Data const& val) -> sm32_1e9_7 {
						return val.extract();
						}) << '\n';
		}
	}

	std::cout << std::flush;
	_exit(0);
}
