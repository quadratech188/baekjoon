#include "../modules/FastIO2.h"
#include <stack>

bool loop() {
	uint n;
	Fast::cin >> n;

	if (n == 0) return false;

	std::vector<int> values(n + 2);
	values[0] = 0;
	values[n + 1] = 0;
	
	for (uint i = 1; i < n + 1; i++) {
		Fast::cin >> values[i];
	}

	int64_t max = 0;

	std::stack<std::pair<uint, uint>> stack;
	stack.emplace(0, values[0]);

	for (uint i = 1; i < values.size(); i++) {
		if (values[i - 1] < values[i]) {
			stack.emplace(i, values[i]);
			continue;
		}
		if (values[i - 1] == values[i])
			continue;

		uint prev_left;
		while (true) {
			auto [left, left_height] = stack.top();

			if (left_height == values[i])
				break;
			
			if (left_height < values[i]) {
				stack.emplace(prev_left, values[i]);
				break;
			}

			stack.pop();
			max = std::max(max, left_height * int64_t(i - left));
			prev_left = left;
		}
	}

	std::cout << max << '\n';

	return true;
}

int main() {
	while (loop()) {}
}
