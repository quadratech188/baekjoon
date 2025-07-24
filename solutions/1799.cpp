#include "../modules/Matrix.h"
#include "../modules/FastIO2.h"

Matrix<char> whitelist;
Matrix<int> threats;

int solve(Int2 after) {
	int result = 0;
	for (Int2 index: threats.bounds()) {
		if (index < after) continue;
		if (whitelist[index] != '1') continue;
		if (threats[index] != 0) continue;
		if ((index.x + index.y) % 2 == 1) continue;

		for (Int2 index2: threats.bounds()) {
			if (std::abs(index.x - index2.x) == std::abs(index.y - index2.y))
				threats[index2] ++;
		}

		result = std::max(result, 1 + solve(index));

		for (Int2 index2: threats.bounds()) {
			if (std::abs(index.x - index2.x) == std::abs(index.y - index2.y))
				threats[index2] --;
		}
	}
	return result;
}

int solve2(Int2 after) {
	int result = 0;
	for (Int2 index: threats.bounds()) {
		if (index < after) continue;
		if (whitelist[index] != '1') continue;
		if (threats[index] != 0) continue;
		if ((index.x + index.y) % 2 == 0) continue;

		for (Int2 index2: threats.bounds()) {
			if (std::abs(index.x - index2.x) == std::abs(index.y - index2.y))
				threats[index2] ++;
		}

		result = std::max(result, 1 + solve2(index));

		for (Int2 index2: threats.bounds()) {
			if (std::abs(index.x - index2.x) == std::abs(index.y - index2.y))
				threats[index2] --;
		}
	}
	return result;
}

int main() {
	uint size;
	Fast::cin >> size;

	whitelist = Matrix<char>(size, size);
	threats = Matrix<int>(size, size);

	Fast::cin >> whitelist;

	std::cout << solve(Int2(0, 0)) + solve2(Int2(0, 0)); 
}
