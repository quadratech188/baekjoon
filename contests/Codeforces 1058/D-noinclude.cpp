#include <iostream>
#include <vector>
#include <cstdint>
using uint = unsigned int;

/*
 *
 * 1 -> 0
 * 1 2 -> 0
 * 1 2 3 -> 15
 *
 * -> a[3] = 15, a[i] = 15 where i < 3
 */

int ask(std::vector<int> values) {
	std::cout << "? " << values.size();
	for (auto v: values) {
		std::cout << ' ' << v;
	}
	std::cout << std::endl;
	int result;
	std::cin >> result;
	return result;
}

void loop() {
	uint n;
	std::cin >> n;

	std::vector<bool> matches(2 * n, false);
	std::vector<int> values(2 * n, -1);

	for (uint ptr = 0; ptr < 2 * n; ptr ++) {
		std::vector<int> query;
		for (uint i = 0; i <= ptr; i ++) {
			if (matches[i]) continue;
			query.push_back(i + 1);
		}
		int result = ask(query);
		if (result != 0) {
			values[ptr] = result;
			matches[ptr] = true;
		}
	}
	for (uint ptr = 0; ptr < 2 * n; ptr ++) {
		if (matches[ptr]) continue;
		std::vector<int> query = {int(ptr + 1)};
		for (uint i = 0; i < 2 * n; i ++) {
			if (matches[i])
				query.push_back(i + 1);
		}
		values[ptr] = ask(query);
	}

	std::cout << '!';
	for (auto x: values) {
		std::cout << ' ' << x;
	}
	std::cout << std::endl;
}

int main() {
	uint t;
	std::cin >> t;
	for (uint i = 0; i < t; i++) loop();
}
