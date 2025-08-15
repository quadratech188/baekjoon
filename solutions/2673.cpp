#include "FastIO2.h"
#include "Matrix.h"
#include "Segment.h"
#include <algorithm>

std::vector<std::pair<int, int>> chords;
Matrix<int> cache(101, 101, -1);

int solve(int l, int r) {
	if (l >= r)
		return 0;

	int& result = cache(l, r);
	if (result != -1)
		return result;
	result = 0;

	for (auto [chord_l, chord_r]: chords) {
		if (chord_l < l || r < chord_r) continue;

		result = std::max(result, 1
				+ solve(l, chord_l - 1)
				+ solve(chord_l + 1, chord_r - 1)
				+ solve(chord_r + 1, r));
	}
	return result;
}

int main() {
	uint n;
	Fast::cin >> n;

	chords.reserve(n);
	for (uint i = 0; i < n; i++) {
		int a, b;
		Fast::cin >> a >> b;
		a --;
		b --;
		if (a > b)
			std::swap(a, b);

		chords.emplace_back(a, b);
	}

	std::cout << solve(0, 99);
}
