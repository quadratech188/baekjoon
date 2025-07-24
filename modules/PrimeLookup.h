#include <array>
#include <vector>

template <int size>
class PrimeLookup {
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
