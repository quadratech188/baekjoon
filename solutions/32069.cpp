#include <bits/stdc++.h>

int main() {
	int n, k;
	long long int l;
	std::cin >> l >> n >> k;

	std::vector<long long int> locations(n);

	for (auto& val: locations)
		std::cin >> val;

	if (k <= n) {
		for (int i = 0; i < k; i++)
			std::cout << "0\n";
		return 0;
	}

	for (int i = 0; i < n; i++)
		std::cout << "0\n";

	k -= n;

	long long int dist = 1;
	int counter = 0;

	while (true) {
		if (locations[0] >= dist) {
			counter++;
			std::cout << dist << '\n';
			if (counter == k) return 0;
		}
		if (l - locations[n - 1] >= dist) {
			counter++;
			std::cout << dist << '\n';
			if (counter == k) return 0;
		}

		for (int i = 0; i < n - 1; i++) {
			if (locations[i + 1] - locations[i] >= 2 * dist + 1) {
				counter++;
				std::cout << dist << '\n';
				if (counter == k) return 0;
				counter++;
				std::cout << dist << '\n';
				if (counter == k) return 0;
			}
			else if (locations[i + 1] - locations[i] >= 2 * dist) {
				counter++;
				std::cout << dist << '\n';
				if (counter == k) return 0;
			}
		}

		dist++;
	}
}
