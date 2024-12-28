#include <iostream>
#include "modules/MillerRabin.h"
#include "modules/FastIO.h"

int main() {
	FastIO();
	int n;
	std::cin >> n;

	int count = 0;

	for (int i = 0; i < n; i++) {
		unsigned long long int s;
		std::cin >> s;
		s = 2 * s + 1;
		if (Math::prime(s)) count++;	
	}
	std::cout << count;
}
