#include <iostream>
#include <ranges>

#include "modules/Input.h"
#include "modules/Compress.h"
#include "modules/SegmentTree.h"
#include "modules/Operators.h"

int main() {
	int n;
	std::cin >> n;
	std::vector<std::pair<int, int>> levels = Input::toVec<std::pair<int, int>>(n);

	std::sort(levels.begin(), levels.end());

	Compress<int> years(levels | std::views::keys);
	SegmentTree<int, Min<int>> tree(levels | std::views::values);
}
