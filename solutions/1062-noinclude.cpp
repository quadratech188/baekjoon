#include <iostream>
#include <vector>

std::vector<std::string> words;

char mandatory[5] = {
	'a', 'n', 't', 'i', 'c'
};
uint default_state = 0;

int solve(uint state, uint ptr, int k) {
	if (k == 0) {
		int result = 0;
		for (auto word: words) {
			bool viable = true;
			for (auto ch: word) {
				if ((1 << (ch - 'a') & state) == 0) {
					viable = false;
					break;
				}
			}
			if (viable) {
				result ++;
			}
		}
		return result;
	}
	if (ptr >= 26) return 0;
	if (state & (1 << ptr)) {
		return solve(state, ptr + 1, k);
	}
	return std::max(solve(state, ptr + 1, k), solve(state | (1 << ptr), ptr + 1, k - 1));
}

int main() {
	uint n, k;
	std::cin >> n >> k;
	k -= 5;

	words.resize(n);
	for (auto& x: words) {
		std::cin >> x;
	}

	for (char ch: mandatory) {
		default_state |= 1 << (ch - 'a');
	}

	std::cout << solve(default_state, 0, k);
}
