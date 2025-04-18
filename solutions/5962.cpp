#include <iostream>
#include <vector>

#include "modules/SegmentTree.h"
#include "modules/InputIterator.h"

std::vector<int> cache;
int n;

int solve(SegmentTree<int>& tree, int index) {
	if (index == n) return 1;

	if (cache[index] != -1) return cache[index];

	if (tree.sum(index, n) < 0) return 0;

	int sum = 0;

	for (int i = index + 1; i <= n; i++) {
		if (tree.sum(index, i) >= 0) {
			sum += solve(tree, i);
			sum %= 1'000'000'009;
		}
}

int main() {
	std::cin >> n;

	cache.resize(n, -1);

	SegmentTree<int> tree(n, InputIterator<int>());

	std::cout << solve(tree, 0);
}
