#include "../modules/FastIO2.h"
#include "../modules/Output.h"
#include <vector>

int main() {
	int n;
	Fast::cin >> n;

	std::vector<int> values(n);
	for (auto& val: values)
		Fast::cin >> val;

	std::vector<int> cache(2 * n - 1);

	for (int sum = 0; sum < 2 * n - 1; sum++) {
		for (int index = sum / 2; index >= 0 && sum - index < n; index--) {
			if (values[index] != values[sum - index]) break;
			cache[sum]++;
		}
	}

	int m;
	Fast::cin >> m;

	for (int i = 0; i < m; i++) {
		int s, e;
		Fast::cin >> s >> e;
		s--;
		e--;

		std::cout << ((e - s) / 2 + 1 <= cache[s + e]) << '\n';
	}
}
