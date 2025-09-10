#include <iostream>
#include <vector>

std::vector<int> cache;

int solve(int n) {
	if (n == 0) return 1;
	if (n == 1) return 1;
	int& result = cache[n];

	if (result != -1)
		return result;

	result = (solve(n - 1) + solve(n - 2)) % 10'007;

	return result;
}

int main() {
	int n;
	std::cin >> n;
	cache.resize(n + 1, -1);

	std::cout << solve(n);
}
