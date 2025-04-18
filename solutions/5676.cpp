#include "modules/SegmentTree.h"
#include "modules/InputRange.h"
#include "modules/Operators.h"
#include "modules/FastIO.h"
#include <iostream>
#include <ranges>

bool loop() {
	int n, k;
	std::cin >> n >> k;

	if (std::cin.eof()) return false;

	SegmentTree<int, Mul<int>> tree{InputRange<int>(n) | std::views::transform([](int val) {
			if (val < 0) return -1;
			if (val == 0) return 0;
			return 1;
			})};

	for (int _ = 0; _ < k; _++) {
		char type;
		std::cin >> type;
		switch(type) {
			case 'C': {
				int i, V;
				std::cin >> i >> V;

				if (V < 0)
					tree.update(i - 1, [](int& val) {val = -1;});
				else if (V == 0)
					tree.update(i - 1, [](int& val) {val = 0;});
				else
				 	tree.update(i - 1, [](int& val) {val = 1;});
				break;
			}
			case 'P': {
				int i, j;
				std::cin >> i >> j;

				int result = tree.sum(i - 1, j);

				if (result < 0) std::cout << '-';
				else if (result == 0) std::cout << '0';
				else std::cout << '+';
			}
		}
	}

	std::cout << '\n';
	return true;
}

int main() {
	FastIO();
	while (loop()) {}
}
