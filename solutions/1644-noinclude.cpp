#include <array>
#include <vector>

template <int size>
struct PrimeLookup {
public:
	constexpr PrimeLookup() {
		is_prime.fill(true);

		is_prime[0] = false;
		is_prime[1] = false;

		for (int i = 2; i * i <= size; i++) {
			if (is_prime[i]) {
				for (int j = i * i; j <= size; j += i)
					is_prime[j] = false;
			}
		}

		for (int i = 2; i <= size; i++) {
			if (is_prime[i])
				primes.push_back(i);
		}
	};

	std::array<bool, size+1> is_prime;
	std::vector<int> primes;
};
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
