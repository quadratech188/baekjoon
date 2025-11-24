#include <iostream>
#include <string>

std::string s1, s2;
int64_t c1, c2;

int64_t cache[100'000][2][2];

int64_t solve(int ptr, bool s1_state = false, bool s2_state = false) {
	if (ptr == -1) {
		return 0;
	}

	int64_t& result = cache[ptr][s1_state][s2_state];

	if (result != -1) {
		return result;
	}

	bool s1_mismatch = ('1' - s1[ptr]) ^ s1_state;
	bool s2_mismatch = ('1' - s2[ptr]) ^ s2_state;

	return result = std::min(
			// No Op 2
			c1 * (s1_mismatch + s2_mismatch) + solve(ptr - 1, '1' - s1[ptr], '1' - s2[ptr]),
			// Op 2
			c2 + c1 * (2 - s1_mismatch - s2_mismatch) + solve(ptr - 1, s1[ptr] - '0', s2[ptr] - '0')
			);
}

int main() {
	int n;
	std::cin >> n >> s1 >> s2 >> c1 >> c2;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				cache[i][j][k] = -1;
			}
		}
	}

	std::cout << solve(n - 1) << '\n';
}
