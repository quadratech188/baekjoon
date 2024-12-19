#include "modules/LazySegmentTree.h"
#include "modules/Input.h"
#include <iostream>

struct Data {
	int value;
	int max;
	int sum;
	int cap;
	int length;

	void update(int action) {
		cap = action;
		if (action >= max) return;

		max = action;
		sum = length * max;
	}

	void resolve(Data& left, Data& right) {

	}
};

std::istream& operator>>(std::istream& is, Data& data) {
	is >> data.value;
}

int main() {
	int n;
	std::cin >> n;

	std::vector<Data> values = Input::inlineToVec<Data>(n);

	LazySegmentTree<Data, int> tree(values);
}
