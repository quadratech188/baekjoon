#include "FastIO2.h"
#include "ModInt.h"
#include "SegmentTree2.h"
#include <iostream>
#include <vector>


int main() {
	uint n;
	Fast::cin >> n;

	// len(subseq) = i -> trees[i - 1]
	std::vector<SegmentTree<sm32_1e9_7>> trees(11, {n + 1, 0});

	for (int val: Fast::cin.to_range<int>(n)) {
		trees[0].update(val, [](sm32_1e9_7& x) {x += 1;});

		for (uint i = 1; i < 11; i++) {
			sm32_1e9_7 delta = trees[i - 1].sum(0, val);
			trees[i].update(val, [delta](sm32_1e9_7& x) {x += delta;});
		}
	}

	std::cout << trees[10].root();
}
