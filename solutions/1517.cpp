#include "FastIO2.h"
#include "Compress.h"
#include "SegmentTree2.h"
#include <algorithm>
#include <iostream>

int main() {
	uint n;
	Fast::cin >> n;

	auto values = Fast::cin.to_vec<int>(n);

	auto values_copy(values);
	std::ranges::sort(values_copy);
	Compress<int> compress(values_copy);

	SegmentTree<int> tree(compress.size());

	int64_t cnt = 0;
	for (auto x: values) {
		uint compressed_x = compress.compress(x);

		if (compressed_x + 1 != compress.size())
			cnt += tree.sum(compressed_x + 1, compress.size());

		tree.update(compressed_x, [](int& k) {k += 1;});
	}
	std::cout << cnt;
}
