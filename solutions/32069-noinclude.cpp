#include <iostream>
#include <vector>
int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(0);
	std::cout.tie(0);

	int l, n, k;
	std::cin >> l >> n >> k;

	std::vector<int> lights(n);

	for (auto& v: lights)
		std::cin >> v;

	std::vector<int> candidates(2 * n);

	for (int i = 0; i < n; i++)
		candidates[2 * i] = candidates[2 * i + 1] = lights[i];

	int cnt = 0;

	std::vector<bool> blacklist(2 * n, false);

	for (int i = 0; i < n; i++)
		std::cout << 0 << '\n';
	k -= n;
	if (k <= 0) return 0;

	int iter = 0;
	while (true) {
		for (int i = 0; i < 2 * n; i++) {
			if (i % 2 == 0)
				candidates[i] --;
			else
			 	candidates[i] ++;

			if (candidates[i] < 0)
				blacklist[i] = true;
			if (candidates[i] > l)
				blacklist[i] = true;
		}

		for (int i = 0; i < 2 * n - 1; i++) {
			if (candidates[i] > candidates[i + 1]) {
				blacklist[i] = true;
				blacklist[i + 1] = true;
			}
			if (candidates[i] == candidates[i + 1]) {
				blacklist[i + 1] = true;
			}
		}
		iter ++;

		for (int i = 0; i < 2 * n; i++) {
			if (!blacklist[i]) {
				std::cout << iter << '\n';
				cnt++;

				if (cnt >= k) return 0;
			}
		}
	}
}
