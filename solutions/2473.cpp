#include "../modules/FastIO2.h"
#include <algorithm>
#include <limits>

int main() {
	uint n;
	Fast::cin >> n;

	auto values = Fast::cin.to_vec<int>(n);

	std::ranges::sort(values);

	int64_t min_diff = std::numeric_limits<int64_t>::max();
	int a, b, c;

	for (uint i = 0; i < values.size() - 1; i++)  {
		uint l_ptr = i + 1;
		uint r_ptr = values.size() - 1;

		while (l_ptr != r_ptr) {
			int64_t sum = (int64_t)values[l_ptr] + values[r_ptr] + values[i];

			if (std::abs(sum) < min_diff) {
				min_diff = std::abs(sum);
				a = values[i];
				b = values[l_ptr];
				c = values[r_ptr];
			}

			if (sum < 0)
				l_ptr ++;
			else
			 	r_ptr --;
		}
	}

	std::cout << a << ' ' << b << ' ' << c;
}
