#include <vector>
#include <iostream>

std::vector<int> cache;

int solve(int n) {
	int& result = cache[n];
	if (result != -1)
		return result;

	result = 1 + solve(n - 1);
	if (n % 3 == 0)
		result = std::min(result, 1 + solve(n / 3));
	if (n % 2 == 0)
		result = std::min(result, 1 + solve(n / 2));

	return result;
}

int main() {
	int n;
	std::cin >> n;
	cache.resize(n + 1, -1);
	cache[1] = 0;

	std::cout << solve(n);
}
