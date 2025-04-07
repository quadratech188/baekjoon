#include "modules/Input.h"
#include "modules/FastIO.h"
#include <algorithm>
#include <iostream>
#include <numeric>

int main() {
	FastIO();
	long long int n, b, c;
	std::cin >> n >> b >> c;

	auto values = Input::toVec<long long int>(n);
	values.push_back(0);
	values.push_back(0);

	if (b <= c) {
		std::cout << b * std::accumulate(values.begin(), values.end(), 0);
		return 0;
	}

	long long int result = 0;
	for (int i = 0; i < n; i++) {
		if (values[i + 1] > values[i + 2]) {
			long long int delta = std::min(values[i], values[i + 1] - values[i + 2]);
			result += (b + c) * delta;

			values[i] -= delta;
			values[i + 1] -= delta;

			delta = std::min(values[i], std::min(values[i + 1], values[i + 2]));
			result += (long long int)(b + 2 * c) * delta;

			values[i] -= delta;
			values[i + 1] -= delta;
			values[i + 2] -= delta;
		}
		else {
			long long int delta = std::min(values[i], std::min(values[i + 1], values[i + 2]));
			result += (b + 2 * c) * delta;

			values[i] -= delta;
			values[i + 1] -= delta;
			values[i + 2] -= delta;

			delta = std::min(values[i], values[i + 1]);
			result += (long long int)(b + c) * delta;

			values[i] -= delta;
			values[i + 1] -= delta;
		}
		result += b * values[i];
		values[i] = 0;
	}

	std::cout << result;
}
