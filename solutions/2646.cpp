#include "modules/Input.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>

bool solution(std::vector<int>& sums, std::vector<int>& lengths, std::vector<bool>& used, int divisor, int total, int index) {
	if (index == divisor + 1) return true;

	if (sums[index] == total) return solution(sums, lengths, used, divisor, total, index + 1);

	for (int i = 0; i < lengths.size(); i++) {
		if (used[i]) continue;

		int length = lengths[i];

		if (sums[index] + length > total) break;

		sums[index] += length;
		used[i] = true;

		if (solution(sums, lengths, used, divisor, total, index)) return true;

		sums[index] -= length;
		used[i] = false;
	}

	return false;
}

int main() {
	int n;
	std::cin >> n;

	std::vector<int> lengths = Input::toVec<int>(n);
	std::sort(lengths.begin(), lengths.end());
	std::vector<bool> used(n);

	int sum = std::accumulate(lengths.begin(), lengths.end(), 0);

	for (int divisor = n; divisor > 0; divisor--) {
		if (sum % divisor != 0) continue;

		std::vector<int> sums(divisor);

		if (solution(sums, lengths, used, divisor, sum / divisor, 0)) {
			std::cout << sum / divisor;
		}
	}
}
