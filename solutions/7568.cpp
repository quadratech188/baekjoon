#include "FastIO2.h"
#include <iostream>

int main() {
	uint n;
	Fast::cin >> n;

	auto men = Fast::cin.to_vec<std::pair<int, int>>(n);

	for (auto man: men) {
		int superior_count = 0;
		for (auto other: men) {
			if (other.first > man.first && other.second > man.second)
				superior_count ++;
		}

		std::cout << superior_count + 1 << ' ';
	}
}
