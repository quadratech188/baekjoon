#include "FastIO2.h"
#include <algorithm>
#include <iostream>

int main() {
	uint n;
	Fast::cin >> n;

	auto times = Fast::cin.to_vec<int>(n);
	std::ranges::sort(times);

	int result = 0;
	int accumulate = 0;
	for (auto time: times) {
		accumulate += time;
		result += accumulate;
	}

	std::cout << result;
}
