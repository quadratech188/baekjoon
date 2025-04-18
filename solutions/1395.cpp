#include <iostream>

#include "modules/FastIO.h"
#include "modules/LazySegmentTree.h"

struct Data {
	int length;
	int on;
	bool flipped;

	Data(): length(1), on(0), flipped(false) {}
	Data(int length, int on, int flipped):
		length(length), on(on), flipped(flipped) {}

	void flip() {
		flipped = !flipped;
		on = length - on;
	}

	Data operator+(const Data& other) const {
		return Data(length + other.length, on + other.on, false);
	}

	void resolve(Data& left, Data& right) {
		if (flipped) {
			left.flip();
			right.flip();
			flipped = false;
		}
	}
};

int main() {
	FastIO();
	int n, m;
	std::cin >> n >> m;

	LazySegmentTree<Data> tree(n);

	for (int i = 0; i < m; i++) {
		int o, s, t;
		std::cin >> o >> s >> t;

		switch(o) {
			case 0:
			tree.update(s - 1, t, [](Data& data) {data.flip();});
			break;
			case 1:
			std::cout << tree.sum(s - 1, t).on << '\n';
		};
	}
}
