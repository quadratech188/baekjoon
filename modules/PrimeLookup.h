#include <array>

template <int size>
class PrimeLookup {
public:
	constexpr PrimeLookup() {
		primes.fill(true);

		primes[0] = false;
		primes[1] = false;

		for (int i = 2; i * i <= size; i++) {
			if (primes[i]) {
				for (int j = i * i; j <= size; j += i)
					primes[j] = false;
			}
		}
	};

	bool operator()(int n) const {
		return primes[n];
	}

private:
	std::array<bool, size+1> primes;

};
