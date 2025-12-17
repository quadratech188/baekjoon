#include <iostream>
#include <vector>
int main() {
	int n, s;
	std::cin >> n >> s;

	std::vector<int> ns(n);
	for (auto& x: ns) {
		std::cin >> x;
	}

	int cnt = 0;
	for (int enc = 1; enc < (1 << n); enc++) {
		int sum = 0;
		for (int i = 0; i < n; i++) {
			if ((1 << i) & enc) {
				sum += ns[i];
			}
		}
		if (sum == s) {
			cnt ++;
		}
	}
	std::cout << cnt << '\n';
}
