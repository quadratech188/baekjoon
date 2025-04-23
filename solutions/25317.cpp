#include "../modules/Input.h"
#include "../modules/Compress.h"
#include "../modules/SegmentTree.h"
#include <iostream>

int main() {
	int q;
	std::cin >> q;

	auto values = Input::toVec<float>(q);

	std::vector<float> sorted(values);

	std::sort(sorted.begin(), sorted.end());
}
