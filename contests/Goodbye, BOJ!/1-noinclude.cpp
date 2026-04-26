#include <iostream>

void repeat(char c, int cnt) {
	for (int i = 0; i < cnt; i++) {
		std::cout << c;
	}
}

int main() {
	std::cin.tie(nullptr);
	std::ios_base::sync_with_stdio(false);
	std::cout.tie(nullptr);

	int n;
	std::cin >> n;

	for (int i = 0; i < n; i++) {
		repeat(' ', 2 * n - 1 - i);
		std::cout << '*';
		repeat(' ', n);
		std::cout << '*';
		repeat(' ', 2 * i + 1);
		std::cout << '*';
		repeat(' ', n - i - 1);
		std::cout << '\n';
	}
	for (int i = 0; i < n; i++) {
		repeat(' ', n - 1 - i);
		std::cout << '*';
		repeat(' ', n + 2 * i + 1);
		std::cout << '*';
		repeat(' ', 2 * n - 1 - 2 * i);
		std::cout << '*';
		repeat(' ', i);
		std::cout << '\n';
	}
}
