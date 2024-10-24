#include "modules/LazySegTree.h"
#include <iostream>
#include <unordered_set>

int main() {
	int n;
	std::cin >> n;



	std::unordered_set<int> candidates;

	for (int i = 0; i < n; i++) {
		int a, b, c, d;
		std::cin >> a >> b >> c >> d;
		candidates.insert(b);
		candidates.insert(d);
	}
}
