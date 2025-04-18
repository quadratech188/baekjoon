#define HACKABLE

#include "modules/SegmentTree.h"
#include "modules/Input.h"
#include <iostream>

int main() {
	int n, m;
	std::cin >> n >> m;

	std::vector<int> counts(n + m + 1);
	
	for (int i = 0; i < n; i++) {
		int height;
		std::cin >> height;
		counts[height]++;
	}

	SegmentTree<int> tree(counts);

	for (int i = 0; i < m; i++) {
		char type;
		std::cin >> type;

		switch (type) {
			case 'F': {
				int c, h;
				std::cin >> c >> h;
				if (tree.sum(h, n + m + 1) <= c) {
					for (int i = n + m - 1; i >= h; i--) {
						int temp = tree.at(i);

						tree.update(i + 1, [&temp](int& val) {val += temp;});
						tree.update(i, [](int& val) {val = 0;});
					}
				}
		  	}
			case 'C': {
				int min, max;
				std::cin >> min >> max;
				std::cout << tree.sum(min, max + 1) << '\n';
		  	}
		}
	}
}
