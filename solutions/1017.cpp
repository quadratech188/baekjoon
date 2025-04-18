#include "modules/PrimeLookup.h"
#include <iostream>
#include <vector>

constexpr PrimeLookup<2000> is_prime;

int main() {
	int n;
	std::cin >> n;

	std::vector<int> evens, odds;
	evens.reserve(n / 2);
	odds.reserve(n / 2);

	int first = std::cin.peek();

	for (int i = 0; i < n; i++) {
		int temp;
		std::cin >> temp;
		if (temp % 2 == 0) evens.push_back(temp);
		else odds.push_back(temp);
	}
	if (evens.size() != odds.size()) {
		std::cout << "-1\n";
		return 0;
	}
}
