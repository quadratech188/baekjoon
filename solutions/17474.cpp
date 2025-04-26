#include "../modules/LazySegmentTree.h"
#include "../modules/InputRange.h"
#include <iostream>
#include <limits>

struct Data {
	int upper_limit;
	long long int sum;
	int max;
	int length;

	Data(int val):
		upper_limit(std::numeric_limits<int>::max()),
		sum(val),
		max(val),
		length(1) {}

	Data operator+(Data const& other) const {

	}

	void resolve(Data& l, Data& r) {

	}
};

int main() {
	int n;
	std::cin >> n;

	LazySegmentTree<Data> tree{InputRange<int>(n)};
}
