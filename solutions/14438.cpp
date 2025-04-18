#include "modules/SegmentTree.h"
#include "modules/Operators.h"
#include "modules/FastIO.h"
#include "modules/InputIterator.h"
#include <iostream>

int main() {
	FastIO();

	int n;
	std::cin >> n;

	SegmentTree<int, Min<int>> tree(n, InputIterator<int>());

	int m;
	std::cin >> m;

	for (int i = 0; i < m; i++) {
		int a, b, c;
		std::cin >> a >> b >> c;

		switch(a) {
			case 1:
				tree.update(b - 1, [&c](int& val){val = c;});
				break;
			case 2:
				std::cout << tree.sum(b - 1, c) << '\n';
		}
	}
}
