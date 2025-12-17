#include <iostream>
#include <string>
#include <ios>

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);
	std::cout.tie(NULL);
	int m;
	std::cin >> m;

	int state = 0;

	for (int i = 0; i < m; i++) {
		std::string inst;
		std::cin >> inst;
		if (inst == "add") {
			int x;
			std::cin >> x;
			state |= (1 << (x - 1));
		}
		else if (inst == "remove") {
			int x;
			std::cin >> x;
			state &= ~(1 << (x - 1));
		}
		else if (inst == "check") {
			int x;
			std::cin >> x;
			std::cout << bool(state & (1 << (x - 1))) << '\n';
		}
		else if (inst == "toggle") {
			int x;
			std::cin >> x;
			state ^= (1 << (x - 1));
		}
		else if (inst == "all") {
			state = (1 << 20) - 1;
		}
		else {
			state = 0;
		}
	}
}
