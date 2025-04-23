#include "../modules/SegmentTree.h"
#include "../modules/FastIO.h"
#include <iostream>

struct Data {
	int delta;
	int minimum;

	Data(char type) {
		switch(type) {
			case '(':
				delta = 1;
				minimum = 0;
				break;
			case ')':
				delta = -1;
				minimum = -1;
		}
	}

	void toggle() {
		if (delta == 1) {
			delta = -1;
			minimum = -1;
		}
		else {
			delta = 1;
			minimum = 0;
		}
	}

	Data(int delta = 0, int minimum = 0):
		delta(delta), minimum(minimum) {}

	Data operator+(Data const& other) const {
		return Data(
				delta + other.delta,
				std::min(minimum, delta + other.minimum)
				);
	}
};

int main() {
	FastIO();
	std::string str;
	std::cin >> str;

	SegmentTree<Data> tree(str);

	int m;
	std::cin >> m;

	int count = 0;

	for (int i = 0; i < m; i++) {
		int index;
		std::cin >> index;

		tree.update(index - 1, [](Data& val) {val.toggle();});

		Data result = tree.root();

		if (result.delta == 0 && result.minimum >= 0)
			count++;
	}

	std::cout << count;
}
