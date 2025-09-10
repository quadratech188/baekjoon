#include "../modules/FastIO2.h"
#include "../modules/Output.h"
#include <algorithm>
#include <iterator>

int main() {
	uint n;
	Fast::cin >> n;

	auto values = Fast::cin.to_vec<int>(n);

	// std::vector<uint> insert_positions(n);

	std::vector<int> lis;
	// uint peak;

	lis.push_back(values[0]);
	// insert_positions[0] = 0;

	// uint max_length = 0;

	for (uint i = 1; i < n; i++) {
		int val = values[i];
		do {
			if (lis[lis.size() - 1] < val) {
				// insert_positions[i] = lis.size();
				lis.push_back(val);
				continue;
			}

			auto ptr = std::ranges::lower_bound(lis, val);
			// insert_positions[i] = ptr - lis.begin();
			*ptr = val;
		} while (false);

		/*
		if (lis.size() > max_length) {
			max_length = lis.size();
			peak = i;
		}
		*/

	}
	/*
	uint target_pos = insert_positions[peak];
	for (uint i = peak; i <= peak; i--) {
		if (insert_positions[i] == target_pos) {
			lis[insert_positions[i]] = values[i];
			target_pos --;
		}
	}
	*/
	std::cout << lis.size();
}
