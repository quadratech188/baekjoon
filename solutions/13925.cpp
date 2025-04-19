#include "../modules/FastIO.h"
#include "../modules/InputRange.h"
#include "../modules/LazySegmentTree.h"
#include "../modules/ModInt.h"
#include <iostream>

struct Update {
	sm32_1e9_7 a, b;
};

struct Data {
	sm32_1e9_7 a;
	sm32_1e9_7 b;
	sm32_1e9_7 _value;
	size_t length;

	Data(sm32_1e9_7 value = 0, size_t length = 1):
		a(1), b(0), _value(value), length(length) {}
	
	constexpr sm32_1e9_7 value() const {
		return this->a * this->_value + this->b * this->length;
	}

	Data operator+(const Data& other) const {
		return Data(
				value() + other.value(),
				length + other.length
				);
	}

	inline void update(Update update) {
		a *= update.a;
		b *= update.a;
		b += update.b;
	}

	inline void resolve(Data& child1, Data& child2) {

		child1.a *= a;
		
		child1.b *= a;
		child1.b += b;

		child2.a *= a;
		
		child2.b *= a;
		child2.b += b;

		_value = value();
		a = 1;
		b = 0;
	}
};

int main() {
	FastIO();
	int n;
	std::cin >> n;

	LazySegmentTree<Data> root{InputRange<int>(n)};

	int m;
	std::cin >> m;

	for (int i = 0; i < m; i++) {
		int type;
		size_t x, y;
		std::cin >> type >> x >> y;
		if (type != 4) {
			Update update;
			int v;
			std::cin >> v;
			switch(type) {
				case 1:
					update = {1, v}; break;
				case 2:
					update = {v, 0}; break;
				case 3:
					update = {0, v}; break;
			}
			root.update(x - 1, y, [update](Data& val) {
					val.update(update);
					});
		}
		else {
			std::cout << root.sum(x - 1, y).value() << '\n';
		}
	}
}
