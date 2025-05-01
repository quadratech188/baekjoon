#include <cstdio>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "../modules/ModInt.h"

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

sm32_1e9_7 current_hash;

struct custom_hash {
	sm32_1e9_7 operator()(std::string_view view) const {
		return current_hash;
	}
};

int main() {
	FastIO();
	int l;
	std::cin >> l;
	std::string string;
	std::cin >> string;
	std::string_view view(string);
	sm32_1e9_7 p = 29;

	std::vector<sm32_1e9_7> powers(l + 1);

	powers[0] = 1;

	for (int i = 1; i < l + 1; i++) {
		powers[i] = p * powers[i - 1];
	}

	int left = 1;
	int right = l;

	int max = -1;

	while (left <= right) {
		int center = (left + right) / 2;

		std::unordered_set<std::string_view, custom_hash> set;

		bool result = false;

		current_hash = 0;

		for (int i = 0; i < center; i++) {
			current_hash = p * current_hash + string[i];
		}

		set.insert(view.substr(0, center));

		for (int i = 1; i <= l - center; i++) {
			current_hash = p * current_hash + string[i + center - 1] - powers[center] * string[i - 1];

			if (set.contains(view.substr(i, center))) {
				result = true;
				break;
			}
			set.insert(view.substr(i, center));

		}

		if (result) {
			max = center;
			left = center + 1;
		}
		else
			right = center - 1;
	}

	if (max == -1)
		std::cout << '0';
	else
		std::cout << max;
}

