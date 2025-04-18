#include "modules/Input.h"
#include "modules/FastIO.h"
#include <iostream>
#include <limits>

int main() {
	FastIO();
	long long int n, m;
	std::cin >> n >> m;


	std::vector<long long int> periods = Input::toVec<long long int>(m);

	if (n < m) {
		std::cout << n;
		return 0;
	}

	long long int left = 0;
	long long int right = std::numeric_limits<long long int>::max();

	long long int result;
	while (left <= right) {
		long long int children = 0;
		long long int center = (left + right) / 2;

		long long int sum = 0;
		for (auto period: periods)
			sum += 1 + (center / period);

		if (sum >= n) {
			right = center - 1;
			result = center;
		}
		else
		 	left = center + 1;
	}
	long long int sum = m;
	for (auto period: periods)
		sum += (result - 1) / period;

	for (long long int i = 0; i < m; i++) {
		if (left % periods[i] == 0)
			sum++;

		if (sum == n) {
			std::cout << i + 1;
			return 0;
		}
	}
}
