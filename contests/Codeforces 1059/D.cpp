#include "CodeForces.h"
#include "FastIO2.h"
#include <iostream>

int query(char type, uint l, uint r) {
	std::cout << type << ' ' << l + 1 << ' ' << r << std::endl;
	int ans;
	Fast::cin >> ans;
	return ans;
}

void submit(uint l, uint r) {
	std::cout << "! " << l + 1 << ' ' << r << std::endl;
}

void solve(uint l, uint r, int64_t prev_sum) {
	if (l + 1 == r) {
		submit(l, r);
		return;
	}
	uint middle = (l + r) / 2;

	int64_t left_delta = query('2', l, middle) - query('1', l, middle);
	int64_t right_delta = prev_sum - left_delta;

	if (left_delta != 0 && right_delta != 0) {
		submit(middle - left_delta, middle + right_delta);
		return;
	}

	if (left_delta != 0) {
		return solve(l, middle, left_delta);
	}
	if (right_delta != 0) {
		return solve(middle, r, right_delta);
	}
}

void loop() {
	uint n;
	Fast::cin >> n;
	int64_t total_sum = query('2', 0, n) - query('1', 0, n);
	solve(0, n, total_sum);
}

int main() {
	uint t;
	Fast::cin >> t;

	for (uint i = 0; i < t; i++) loop();
}
