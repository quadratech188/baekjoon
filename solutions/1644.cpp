#include "../modules/PrimeLookup.h"
#include <cstdint>
#include <iostream>
#include <sys/types.h>

PrimeLookup<4'000'000> lookup;

int main() {
	std::vector<int64_t> partial_sum(lookup.primes.size() + 1);

	for (uint i = 0; i < lookup.primes.size(); i++) {
		partial_sum[i + 1] = partial_sum[i] + lookup.primes[i];
	}

	int64_t target;
	std::cin >> target;

	uint l_ptr = 0;
	uint r_ptr = 0;

	int cnt = 0;
	while (l_ptr != partial_sum.size() - 1) {
		int64_t sum = partial_sum[r_ptr] - partial_sum[l_ptr];
		if (sum == target)
			cnt++;

		if (sum <= target && r_ptr != partial_sum.size() - 1)
			r_ptr ++;
		else
		 	l_ptr ++;
	}

	std::cout << cnt;
}
