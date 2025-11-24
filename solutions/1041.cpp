#include "FastIO2.h"
#include <algorithm>
#include <iostream>
int main() {
	int64_t n, a, b, c, d, e, f;
	Fast::cin >> n >> a >> b >> c >> d >> e >> f;

	if (n == 1) {
		std::cout << a + b + c + d + e + f - std::max({
				a, b, c, d, e, f
				});
		return 0;
	}

	int64_t face = std::min({
			a,
			b,
			c,
			d,
			e,
			f
			});
	
	int64_t edge = std::min({
			a + b,
			b + f,
			f + e,
			e + a,
			e + d,
			d + b,
			b + c,
			c + e,
			d + a,
			a + c,
			c + f,
			f + d
			});

	int64_t corner = std::min({
			e + d + a,
			d + a + b,
			b + a + c,
			c + e + a,
			d + b + f,
			b + c + f,
			e + c + f,
			f + e + d
			});

	std::cout << ((n - 2) * (n - 2) + 4 * (n - 2) * (n - 1)) * face
		+ (4 * (n - 2) + 4 * (n - 1)) * edge
		+ 4 * corner;
}
