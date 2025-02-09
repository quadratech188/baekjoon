#include <iostream>
#include "modules/InputIterator.h"
#include "modules/SegmentTree.h"
#include "modules/FastIO.h"

int main() {
	FastIO();
	int n, m, k;
	std::cin >> n >> m >> k;

	SegmentTree<long long int> tree(n, InputIterator<long long int>());

	for (int i = 0; i < m + k; i++) {
		long long int a, b, c;
		std::cin >> a >> b >> c;
		switch(a) {
			case 1:
				tree.update(b - 1, [c](long long int& val) {
						val = c;
						});
				break;
			case 2:
				std::cout << tree.sum(b - 1, c) << '\n';
		}
	}
}
