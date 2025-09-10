#include <iostream>
#include <vector>


/*
 * a[i] = max(a[i - 3] + s[i - 1], a[i - 2]) + s[i]
 */

int main() {
	int n;
	std::cin >> n;

	std::vector<int> scores(n);
	for (int i = 0; i < n; i++)
		std::cin >> scores[i];

	std::vector<int> cache(n);

	cache[0] = scores[0];
	cache[1] = scores[0] + scores[1];
	cache[2] = std::max(scores[0], scores[1]) + scores[2];

	for (int i = 3; i < n; i++)
		cache[i] = std::max(
				cache[i - 3] + scores[i - 1],
				cache[i - 2]
				) + scores[i];

	std::cout << cache[n - 1];
}
