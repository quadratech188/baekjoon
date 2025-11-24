#include <iostream>
#include <limits>
#include "FastIO2.h"

int main() {
	uint n;
	Fast::cin >> n;
	auto values = Fast::cin.to_vec<int>(n);

	uint ptr = 0;
	int prev = -1;
	int cnt = 0;
	int max = 0;
	while (ptr < n) {
		if (prev > values[ptr]) {
			max = std::max(max, cnt);
			cnt = 0;
		}
		prev = values[ptr];
		ptr ++;
		cnt ++;
	}
	max = std::max(max, cnt);
	cnt = 1;

	ptr = 0;
	prev = std::numeric_limits<int>::max();
	cnt = 0;
	while (ptr < n) {
		if (prev < values[ptr]) {
			max = std::max(max, cnt);
			cnt = 0;
		}
		prev = values[ptr];
		ptr ++;
		cnt ++;
	}
	max = std::max(max, cnt);
	cnt = 1;

	std::cout << max;
}
