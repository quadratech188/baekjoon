#include "../modules/FastIO.h"
#include "../modules/InputRange.h"
#include "../modules/LazySegmentTree.h"
#include "../modules/ModInt.h"
#include <iostream>

struct Data {
	sm32_1e9_7 a;
	sm32_1e9_7 b;
	sm32_1e9_7 _value;
	size_t length;

	Data(sm32_1e9_7 value = 0, size_t length = 1):
		a(1), b(0), _value(value), length(length) {}
	
	inline sm32_1e9_7 value() const {
		return this->a * this->_value + this->b * this->length;
	}

	Data operator+(const Data& other) const {
		return Data(
				value() + other.value(),
				length + other.length
				);
	}
	void resolve(Data& child1, Data& child2) {

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
		int v;
		switch(type) {
			case 1:
				std::cin >> v;
				root.update(x - 1, y, [v](Data& val) {
						val.b += v;
						});
				break;
			case 2:
				std::cin >> v;
				root.update(x - 1, y, [v](Data& val) {
						val.a *= v;
						val.b *= v;
						});
				break;
			case 3:
				std::cin >> v;
				root.update(x - 1, y, [v](Data& val) {
						val.a = 0;
						val.b = v;
						});
				break;
			case 4:
				std::cout << root.sum(x - 1, y).value() << '\n';
		}
	}
}
