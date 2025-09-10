#include <iostream>
#include <vector>

int main() {
	int t;
	std::cin >> t;

	int cache[11];
	cache[0] = 1;
	cache[1] = 1;
	cache[2] = 2;

	for (int i = 3; i < 11; i++)
		cache[i] = cache[i - 1] + cache[i - 2] + cache[i - 3];

	for (int i = 0; i < t; i++) {
		int n;
		std::cin >> n;
		std::cout << cache[n] << '\n';
	}
}
