#include "../modules/FastIO2.h"
#include <algorithm>
#include <limits>
#include <vector>

int main() {
	uint n;
	Fast::cin >> n;

	std::vector<int> values(n);
	for (auto& val: values)
		Fast::cin >> val;

	std::ranges::sort(values);

	int max_diff = std::numeric_limits<int>::max();
	int a, b;

	uint l_ptr = 0;
	uint r_ptr = n - 1;

	while (l_ptr != r_ptr) {
		int sum = values[l_ptr] + values[r_ptr];
		if (std::abs(sum) < max_diff) {
			a = values[l_ptr];
			b = values[r_ptr];
			max_diff = std::abs(sum);
		}

		if (sum < 0)
			l_ptr++;
		else
		 	r_ptr--;
	}

	std::cout << a << ' ' << b;
}
