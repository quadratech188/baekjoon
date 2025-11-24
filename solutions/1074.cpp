#include "FastIO2.h"
#include <iostream>

uint solve(uint N, uint r, uint c) {
	if (N == 0) return 0;

	uint half_len = 1 << (N - 1);

	return (2 * (r >= half_len) + (c >= half_len)) * half_len * half_len + solve(N - 1, r % half_len, c % half_len);
}

int main() {
	uint N, r, c;
	Fast::cin >> N >> r >> c;
	std::cout << solve(N, r, c);
}
