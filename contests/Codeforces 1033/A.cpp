#include <array>
#include <iostream>
#include "../../modules/Vec2.h"

bool eval(Int2 a, Int2 b, Int2 c) {
	// | | |
	if (a.y == b.y && b.y == c.y && a.x + b.x + c.x == a.y)
		return true;

	// -
	// -
	// -
	if (a.x == b.x && b.x == c.x && a.y + b.y + c.y == a.x)
		return true;

	// - |
	// - |
	if (a.x == b.x && a.y + b.y == c.y && a.x + c.x == c.y)
		return true;
	// --
	// ||
	if (a.y == b.y && a.x + b.x == c.x && a.y + c.y == c.x)
		return true;

	return false;
}

void loop() {
	Int2 a, b, c;
	std::cin >> a >> b >> c;

	if (eval(a, b, c)
			|| eval(a, c, b)
			|| eval(b, a, c)
			|| eval(b, c, a)
			|| eval(c, a, b)
			|| eval(c, b, a)) {
		std::cout << "YES\n";
	}
	else {
		std::cout << "NO\n";
	}
}

int main() {
	int t;
	std::cin >> t;
	for (int i = 0; i < t; i++) loop();

}
