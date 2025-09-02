#include <iostream>
#include <string>
#include <vector>

int main() {
	std::string str;
	std::cin >> str;

	std::vector<int> plus_sums;

	int val = 0;
	int plus_sum = 0;

	for (auto ch: str) {
		if (ch == '+' || ch == '-') {
			plus_sum += val;
			val = 0;
			if (ch == '-') {
				plus_sums.push_back(plus_sum);
				plus_sum = 0;
			}
		}
		else {
			val = 10 * val + (ch - '0');
		}
	}
	plus_sum += val;
	plus_sums.push_back(plus_sum);

	int result = plus_sums[0];

	for (size_t i = 1; i < plus_sums.size(); i++)
		result -= plus_sums[i];

	std::cout << result;
}
