#include "../modules/FastIO2.h"
#include "../modules/FastIO.h"
#include <algorithm>
#include <vector>

int main() {
	FastIO();
	uint n;
	Fast::cin >> n;

	std::vector<uint> values(n);
	for (auto& val: values)
		Fast::cin >> val;

	uint max = std::ranges::max(values);

	std::vector<bool> exists(max + 1);
	std::vector<int> scores(max + 1);

	for (auto const& val: values)
		exists[val] = true;

	for (auto const& val: values) {
		for (uint j = 2 * val; j <= max; j += val) {
			if (exists[j]) {
				scores[val]++;
				scores[j]--;
			}
		}
	}

	for (auto const& val: values)
		std::cout << scores[val] << ' ';
}
