#include "FastIO2.h"
#include <cstdint>
#include <iostream>
#include <queue>
#include <vector>
#include "SegmentTree.h"

void loop() {
	uint n;
	Fast::cin >> n;

	std::vector<std::vector<uint>> waiting(n);

	for (uint i = 0; i < n; i++) {
		uint a_i;
		Fast::cin >> a_i;
		waiting[a_i - 1].push_back(i);
	}

	std::vector<uint> result(n);

	std::priority_queue<int> stack;

	for (int i = n - 1; i >= 0; i--) {
		for (auto person: waiting[i]) {
			stack.push(person);
		}

		if (stack.empty()) {
			std::cout << "-1\n";
			return;
		}

		result[i] = stack.top();
		stack.pop();
	}

	SegmentTree<int> tree(n);

	int64_t inversions = 0;
	for (uint i = 0; i < n; i++) {
		inversions += tree.sum(result[i], n);
		tree.update(result[i], [](int& x) {x += 1;});
	}

	std::cout << inversions << '\n';
}

int main() {
	uint t;
	Fast::cin >> t;
	for (uint i = 0; i < t; i++) loop();
}
