#include "FastIO2.h"

int main() {

	uint n;
	Fast::cin >> n;
	auto values = Fast::cin.to_vec<uint>(n);

	std::vector<int> deltas(n);

	for (uint i = 0; i < n; i++) {
		values[i] --;
		deltas[i] = n + values[i] - i;
		if (deltas[i] >= n) {
			deltas[i] -= n;
		}

		if (2 * deltas[i] > (int)n) {
			// anticlockwise
			deltas[i] -= n;
		}
	}
}
