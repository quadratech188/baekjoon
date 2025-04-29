#include "../modules/FastIO.h"
#include "../modules/PrimeLookup.h"
#include <iostream>

PrimeLookup<2 * 123456> lookup;

int main() {
	FastIO();
	while (true) {
		int n;
		std::cin >> n;
		if (n == 0) break;

		int cnt = 0;
		for (int i = n + 1; i <= 2 * n; i++) {
			if (lookup(i)) cnt++;
		}

		std::cout << cnt << '\n';
	}
}
