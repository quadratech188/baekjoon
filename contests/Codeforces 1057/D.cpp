#include "CodeForces.h"
#include "Matrix.h"
#include "FastIO2.h"
#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

std::vector<int> status;

Matrix<int64_t> cache(0, 0);

int64_t solve(uint i) {
    if (i >= status.size()) return 0;

    int64_t& result = cache(i,0);
    if (result != -1) return result;
    result = std::numeric_limits<int64_t>::max()/2;

    if (i+1 < status.size()) {
        int64_t cost2 = std::abs(status[i] - status[i+1]);

        result = std::min(result, cost2 + solve(i+2));
    }

    if (i+2 < status.size()) {
        int mx = std::max({status[i], status[i+1], status[i+2]});
        int mn = std::min({status[i], status[i+1], status[i+2]});

        int64_t cost3 = mx - mn;
        result = std::min(result, cost3 + solve(i+3));
    }

    return result;
}

void loop() {
	uint n;
	Fast::cin >> n;

	cache = Matrix<int64_t>(n, 2, -1);
	status = Fast::cin.to_vec<int>(n);

	int64_t result = solve(0);

	int front = status.front();
	for (uint i = 0; i < n - 1; i++) {
		status[i] = status[i + 1];
	}
	status.back() = front;

	cache = Matrix<int64_t>(n, 2, -1);
	result = std::min(result, solve(0));

	front = status.front();
	for (uint i = 0; i < n - 1; i++) {
		status[i] = status[i + 1];
	}
	status.back() = front;

	cache = Matrix<int64_t>(n, 2, -1);
	result = std::min(result, solve(0));

	std::cout << result << '\n';
}

int main() {
	uint t;
	Fast::cin >> t;
	for (uint i = 0; i < t; i++) loop();
}
