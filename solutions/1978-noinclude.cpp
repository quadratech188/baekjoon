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

PrimeLookup<1000000> lookup;

int main() {
	FastIO();
	int n;
	std::cin >> n;
	int cnt = 0;
	for (int i = 0; i < n; i++) {
		int a;
		std::cin >> a;
		if (lookup(a)) cnt++;
	}
	std::cout << cnt;
}
