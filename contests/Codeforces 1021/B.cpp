#include "../../modules/Input.h"
#include "../../modules/FastIO.h"
#include <algorithm>
#include <iostream>

/*
 * when is f(a) minimal?
 * 1) when f'(x) > 0 left of x, f'(x) < 0 right of x
 * 		n is odd, and x = (n + 1) / 2
 * 2) when f'(a) == 0
 * 		n is even, and n / 2 <= x <= n / 2 + 1
 */

void loop() {
	int n, k;
	std::cin >> n >> k;

	auto values = Input::toVec<int>(n);

	std::sort(values.begin(), values.end());

	/*
	 * 1) (n - k) % 2 == 0
	 * 		(n - k) / 2 <= x <= (n - k) / 2 + 1
	 *
	 * 	remove left values:
	 * 	(n + k) / 2 <= x <= (n + k) / 2 + 1
	 * 	remove right values:
	 * 	(n - k) / 2 <= x <= (n - k) / 2 + 1
	 * 	cases = (n - k) / 2 to (n + k) / 2 + 1
	 *
	 * 	2) (n - k) % 2 == 1
	 * 		x = (n - k + 1) / 2
	 * 	remove left values:
	 * 	x = (n + k + 1) / 2
	 * 	remove right values:
	 * 	x = (n - k + 1) / 2
	 */

	int min, max;

	if ((n - k) % 2 == 0) {
		min = (n - k) / 2;
		max = (n + k) / 2 + 1;
	}
	else {
		min = (n - k + 1) / 2;
		max = (n + k + 1) / 2;
	}

	std::cout << values[max - 1] - values[min - 1] + 1 << '\n';
}

int main() {
	FastIO();
	int t;
	std::cin >> t;

	for (int i = 0; i < t; i++) loop();
}
