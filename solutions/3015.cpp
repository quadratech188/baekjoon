#include "FastIO2.h"
#include <cstdint>
#include <iostream>
#include <stack>

int main() {
	uint n;
	Fast::cin >> n;

	std::stack<std::pair<int, uint>> stack;

	int64_t cnt = 0;
	for (int person: Fast::cin.to_range<int>(n)) {
		int duplicates = 0;

		while (!stack.empty() && stack.top().first <= person) {
			cnt += stack.top().second;

			if (stack.top().first == person)
				duplicates = stack.top().second;

			stack.pop();

		}

		if (!stack.empty())
			cnt ++;
		stack.emplace(person, duplicates + 1);
	}

	std::cout << cnt;
}
