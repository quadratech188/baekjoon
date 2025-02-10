#include "modules/Vec2.h"
#include "modules/SegmentTree.h"
#include "modules/Input.h"
#include "modules/FastIO.h"
#include "modules/Compress.h"
#include <iostream>
#include <ranges>

void loop() {
	int n;
	std::cin >> n;

	std::vector<Int2> islands = Input::toVec<Int2>(n);
	std::vector<int> y_values;
	y_values.reserve(n);
	for (Int2& island: islands)
		y_values.push_back(island.y);

	std::sort(y_values.begin(), y_values.end());

	Compress<int> y(y_values);

	std::sort(islands.begin(), islands.end(), [](Int2& l, Int2& r) {
			if (l.x != r.x) return l.x < r.x;
			return l.y > r.y;
			});

	SegmentTree<int> tree(y.size());

	long long int sum = 0;
	for (Int2& island: islands) {
		int compressed_y = y.compress(island.y);
		sum += tree.sum(compressed_y, tree.size());
		tree.update(compressed_y, [](int& val) {val++;});
	}
	std::cout << sum << '\n';
}

int main() {
	FastIO();
	int n;
	std::cin >> n;
	for (int i = 0; i < n; i++)
		loop();
}
