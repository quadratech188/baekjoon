#include <iostream>
int main() {
	int64_t cache[101] = {0, 1, 1, 1, 2, 2};

	for (int i = 5; i < 101; i++)
		cache[i] = cache[i - 1] + cache[i - 5];

	int t;
	std::cin >> t;

	for (int i = 0; i < t; i++) {
		int n;
		std::cin >> n;
		std::cout << cache[n] << '\n';
	}
}
