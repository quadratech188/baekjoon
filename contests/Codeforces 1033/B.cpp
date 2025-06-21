#include "../../modules/Vec2.h"
#include <array>


void loop() {
	int n, s;
	std::cin >> n >> s;

	int result = 0;
	for (int i = 0; i < n; i++) {
		Int2 dir, pos;
		std::cin >> dir >> pos;

		if (dir.x == dir.y) {
			if (pos.x == pos.y) result++;
		}
		else {
			if (pos.x + pos.y == s) result++;
		}
	}

	std::cout << result << '\n';
}

int main() {
	int t;
	std::cin >> t;
	for (int i = 0; i < t; i++) loop();
}
