#include "../modules/PrimeLookup.h"
#include "../modules/FastIO.h"
#include <iostream>

PrimeLookup<1'000'000> lookup;

int main() {
	FastIO();
	int m, n;
	std::cin >> m >> n;
	for (int i = m; i <= n; i++) {
		if (lookup(i)) std::cout << i << '\n';
	}
}
