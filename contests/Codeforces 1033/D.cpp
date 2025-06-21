#include "../../modules/FastIO.h"
#include "../../modules/FastIO2.h"
#include "../../modules/Math.h"
#include "../../modules/ModInt.h"

void loop() {
	uint32_t a_raw, b_raw, k_raw;
	Fast::cin >> a_raw >> b_raw >> k_raw;
	sm32_1e9_7 a = a_raw;
	sm32_1e9_7 b = b_raw;
	sm32_1e9_7 k = k_raw;

	sm32_1e9_7 n = (a - 1) * k + 1;

	sm32_1e9_7 m = (b - 1) * k * Math::C(n, a) + 1;

	std::cout << n << ' ' << m << '\n';
}

int main() {
	FastIO();
	uint32_t t;
	Fast::cin >> t;
	for (uint32_t i = 0; i < t; i++) loop();
}
