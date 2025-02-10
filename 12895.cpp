#include "modules/LazySegmentTree.h"
#include "modules/FastIO.h"
#include <functional>
#include <iostream>

struct Data {
	Data():
		colors(1), one_color(false) {}
	Data(int color):
		colors(color), one_color(false) {}
	int colors;
	bool one_color;

	Data operator+(const Data& other) {
		return Data(colors | other.colors);
	}
	void resolve(Data& left, Data& right) {
		if (one_color) {
			left.colors = colors;
			left.one_color = true;
			right.one_color = true;
			right.colors = colors;
			one_color = false;
		}
	}
};

int main() {
	FastIO();
	int n, t, q;
	std::cin >> n >> t >> q;

	LazySegmentTree<Data> tree(n);

	for (int i = 0; i < q; i++) {
		char type;
		std::cin >> type;
		if (type == 'C') {
			int x, y, z;
			std::cin >> x >> y >> z;
			if (x > y) std::swap(x, y);
			tree.update(x - 1, y, [z](Data& val) {
					val.colors = 1 << (z - 1);
					val.one_color = true;
					});
		}
		else {
			int x, y;
			std::cin >> x >> y;
			if (x > y) std::swap(x, y);
			int result = tree.sum(x - 1, y).colors;
			int cnt = 0;
			for (int i = 0; i < t; i++) {
				if (result & (1 << i)) cnt++;
			}
			std::cout << cnt << '\n';
		}
	}
}
