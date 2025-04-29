#include <array>
#include <cstdio>
#include <iostream>

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

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

PrimeLookup<1'000'000> lookup;

int main() {
	FastIO();
	int m, n;
	std::cin >> m >> n;
	for (int i = m; i <= n; i++) {
		if (lookup(i)) std::cout << i << '\n';
	}
}
