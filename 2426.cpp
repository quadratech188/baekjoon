#include <iostream>
#include <unordered_set>
#include <vector>
#include "modules/LazySegmentTree.h"

const int maxHeight = 1000000000;

std::vector<int> indexToHeight(100000);

struct Count {

};

int main() {
	int n, m;
	std::cin >> n >> m;

	std::unordered_set<int> heights;
	std::vector<int> heights;

	for (int _ = 0; _ < n; _++) {
		int height;
		std::cin >> height;
		heights.insert(height);
	}


}
