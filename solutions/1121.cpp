#include "modules/Input.h"
#include <iostream>
#include <numeric>
int main() {
	int n;
	std::cin >> n;

	std::vector<int> lengths = Input::toVec<int>(n);

	int sum = std::accumulate(lengths.begin(), lengths.end(), 0);

	int k;
	std::cin >> k;
}
