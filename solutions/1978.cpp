#include "../modules/PrimeLookup.h"
#include "../modules/FastIO.h"
#include <iostream>

PrimeLookup<1000> lookup;

int main() {
	FastIO();
	int n;
	std::cin >> n;
	int cnt = 0;
	for (int i = 0; i < n; i++) {
		int a;
		std::cin >> a;
		if (lookup(a)) cnt++;
	}
	std::cout << cnt;
}
