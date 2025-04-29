#include <array>
#include <cstdio>
#include <iostream>

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

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

PrimeLookup<2 * 123456> lookup;

int main() {
	FastIO();
	while (true) {
		int n;
		std::cin >> n;
		if (n == 0) break;

		int cnt = 0;
		for (int i = n + 1; i <= 2 * n; i++) {
			if (lookup(i)) cnt++;
		}

		std::cout << cnt << '\n';
	}
}
