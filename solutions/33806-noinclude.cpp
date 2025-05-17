#include <cstdint>
#include <iostream>

int main() {
	int n, m;
	std::cin >> n >> m;

	if (m == 2 * n) {
		std::cout << "0\n";
		for (int i = 1; i < (1 << n); i++) {
			for (int k = n - 1; k >= 0; k--) {
				if (i & (1 << k))
					std::cout << '1';
				else
				 	std::cout << '2';
			}
			std::cout << '\n';
		}
		return 0;
	}
}
