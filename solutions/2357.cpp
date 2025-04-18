#include "modules/SegmentTree.h"
#include "modules/Operators.h"
#include "modules/InputIterator.h"
#include "modules/FastIO.h"
#include <iostream>
#include <istream>

struct Data {
	long long int min, max;

	Data operator+(const Data& other) const {
		return Data {std::min(min, other.min), std::max(max, other.max)};
	}
};

std::istream& operator>>(std::istream& is, Data& data) {
	is >> data.min;
	data.max = data.min;
	return is;
};

std::ostream& operator<<(std::ostream& os, const Data& data) {
	os << data.min << ' ' << data.max;
	return os;
}

int main() {
	FastIO();
	int n, m;
	std::cin >> n >> m;

	SegmentTree<Data> tree(n, InputIterator<Data>());

	for (int i = 0; i < m; i++) {
		int a, b;
		std::cin >> a >> b;

		std::cout << tree.sum(a - 1, b) << '\n';
	}
}
