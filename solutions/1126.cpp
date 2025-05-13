#include <iostream>
#include <limits>
#include <vector>
#include "../modules/Input.h"
#include "../modules/FastIO.h"
#include "../modules/Matrix.h"

std::vector<int> blocks;

Matrix<int> cache(50, 500'000, std::numeric_limits<int>::min());

int solution(size_t index, int delta) {
	if (index == blocks.size()) {
		if (delta == 0) return 0;
		return std::numeric_limits<int>::min();
	}

	int& result = cache(index, delta);

	if (result == std::numeric_limits<int>::min()) {
		result = std::max(result, solution(index + 1, delta));
		result = std::max(result, solution(index + 1, delta + blocks[index]));
		if (delta < blocks[index])
			result = std::max(result, delta + solution(index + 1, blocks[index] - delta));
		else
		 	result = std::max(result, blocks[index] + solution(index + 1, delta - blocks[index]));
	}
	return result;
}

int main() {
	FastIO();
	int n;
	std::cin >> n;
	blocks = Input::toVec<int>(n);

	int result = solution(0, 0);
	if (result <= 0) std::cout << "-1";
	else std::cout << result;
}
