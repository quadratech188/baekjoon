#include <algorithm>
#include <cstdio>
#include <iostream>
#include <istream>
#include <iterator>
#include <vector>

template< typename T1, typename T2>
std::istream& operator>>(std::istream& is, std::pair<T1, T2>& data) {
	is >> data.first >> data.second;
	return is;
}

template <typename T>
std::istream& operator>>(std::istream& is, std::vector<T>& data) {
	for (auto& element: data)
		is >> element;
	return is;
}

namespace Input {
	template<typename T>
	inline std::vector<T> toVec(size_t n) {
		std::vector<T> result(n);
		std::cin >> result;

		return result;
	}
}

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

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
