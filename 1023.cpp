#include "modules/Matrix.h"
#include "modules/FastIO.h"

Matrix<long long int> cache(52, 52, -1);

long long int count(int length, int delta) {
	if (delta < 0) return ((long long int)1 << length);
	if (length == 0) return delta == 0? 0: 1;

	if (cache(length, delta) == -1)
		cache(length, delta) = count(length - 1, delta - 1) + count(length - 1, delta + 1);

	return cache(length, delta);
}

int main() {
	FastIO();
	long long int n, k;
	std::cin >> n >> k;

	int length = n;
	int delta = 0;
	bool redeemable = true;

	if (k >= count(n, 0)) {
		std::cout << "-1\n";
		return 0;
	}

	while (length > 1) {
		long long int boundary = redeemable? count(length - 1, delta + 1): (long long int)1 << (length - 1);
		if (k >= boundary) {
			k -= boundary;
			length --;
			delta --;
			std::cout << ')';
		}
		else {
			length --;
			delta ++;
			std::cout << '(';
		}
		if (delta < 0) redeemable = false;
	}
	if (k == 1) std::cout << ")\n";
	else std::cout << "(\n";
}
