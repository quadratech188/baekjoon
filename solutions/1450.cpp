#include "modules/Input.h"
#include "modules/FastIO.h"
#include <algorithm>
#include <iostream>
int main() {
	FastIO();
	int n, c;
	std::cin >> n >> c;

	std::vector<int> masses = Input::toVec<int>(n);

	int left = n / 2;
	int right = n - left;

	std::vector<long long int> left_sums;
	left_sums.reserve(1 << left);

	std::vector<long long int> right_sums;
	right_sums.reserve(1 << right);

	for (int i = 0; i < 1 << left; i++) {
		long long int sum = 0;
		for (int j = 0; j < left; j++) {
			if (i & (1 << j)) sum += masses[j];
		}
		left_sums.push_back(sum);
	}
	for (int i = 0; i < 1 << right; i++) {
		long long int sum = 0;
		for (int j = 0; j < right; j++) {
			if (i & (1 << j)) sum += masses[left + j];
		}
		right_sums.push_back(sum);
	}

	std::sort(right_sums.begin(), right_sums.end());

	long long int count = 0;
	for (long long int l: left_sums) {
		count += std::upper_bound(right_sums.begin(), right_sums.end(), c - l) - right_sums.begin();
	}
	std::cout << count;
}
