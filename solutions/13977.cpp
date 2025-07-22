#include "../modules/FastIO2.h"
#include "../modules/FastIO.h"
#include "../modules/Math.h"
#include "../modules/ModInt.h"

sm32_1e9_7 lookup[4'000'001];

int main() {
	FastIO();
	uint M;
	Fast::cin >> M;

	lookup[0] = 1;
	for (size_t i = 1; i < 4'000'001; i++)
		lookup[i] = lookup[i - 1] * i;

	for (uint i = 0; i < M; i++) {
		uint n, k;
		Fast::cin >> n >> k;

		std::cout << lookup[n] * lookup[k].inverse() * lookup[n - k].inverse() << '\n';
	}
}
