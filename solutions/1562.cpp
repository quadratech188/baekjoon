#include <cstdint>
#include <iostream>

constexpr int MOD = 1'000'000'000;
int cache[100][10][1 << 10];

int solve(uint len, uint8_t prev, uint16_t used) {
	if (len == 0) {
		return used == (1 << 10) - 1;
	}
	int& result = cache[len][prev][used];
	if (result != -1) {
		return result;
	}

	result = 0;

	if (0 < prev) {
		(result += solve(len - 1, prev - 1, used | (1 << (prev - 1)))) %= MOD;
	}
	if (prev < 9) {
		(result += solve(len - 1, prev + 1, used | (1 << (prev + 1)))) %= MOD;
	}
	return result;
}

int main() {
	uint len;
	std::cin >> len;

	for (uint i = 0; i < len; i++) {
		for (uint8_t prev = 0; prev < 10; prev++) {
			for (uint16_t used = 0; used < (1 << 10); used++) {
				cache[i][prev][used] = -1;
			}
		}
	}

	int result = 0;
	for (uint8_t ch = 1; ch <= 9; ch ++) {
		(result += solve(len - 1, ch, 1 << ch)) %= MOD;
	}

	std::cout << result << '\n';
}
