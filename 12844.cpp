#include <iostream>

#include "modules/LazySegmentTree.h"
#include "modules/InputIterator.h"
#include "modules/FastIO.h"

struct Data {
	Data():
		_value(0), length(1), delta(0) {}
	Data(int value):
		_value(value), length(1), delta(0) {}
	Data(int value, int length):
		_value(value), length(length), delta(0) {}

	int _value;
	int length;
	int delta;

	void apply(int k) {
		delta ^= k;
	}

	int value() const {
		return length % 2 == 0? _value: _value ^ delta;
	}

	void resolve(Data& left, Data& right) {
		left.apply(delta);
		right.apply(delta);
		_value = value();
		delta = 0;
	}

	Data operator+(const Data& other) const {
		return Data(value() ^ other.value(), length + other.length);
	}
};

int main() {
	FastIO();
	int n;
	std::cin >> n;

	LazySegmentTree<Data> tree(n, InputIterator<int>());

	int m;
	std::cin >> m;
	for (int i = 0; i < m; i++) {
		int a, b, c;
		std::cin >> a >> b >> c;
		if (a == 1) {
			int k;
			std::cin >> k;
			tree.update(b, c + 1, [k](Data& data) {
					data.apply(k);
					});
		}
		else {
			std::cout << tree.sum(b, c + 1).value() << '\n';
		}
	}
}
