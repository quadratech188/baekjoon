#include "../modules/Vec2.h"
#include "../modules/FastIO2.h"
#include <iomanip>
#include <ios>
#include <utility>

int main() {
	std::cout << std::fixed << std::setprecision(9);

	Int2 a1, a2, b1, b2;
	Fast::cin >> a1 >> a2 >> b1 >> b2;

	if ((a2 - a1).cross(b2 - b1) != 0) {
		// a1 + (a2 - a1) x = b1 + (b2 - b1) y
		
		// (a2 - a1) cross (b2 - b1) x = (b1 - a1) cross (b2 - b1)
		double x = (double)(b1 - a1).cross(b2 - b1) / (a2 - a1).cross(b2 - b1);

		// (a1 - b1) cross (a2 - a1) = (b2 - b1) cross (a2 - a1) y
		double y = (double)(a1 - b1).cross(a2 - a1) / (b2 - b1).cross(a2 - a1);

		if (0 <= x && x <= 1 && 0 <= y && y <= 1) {
			std::cout << "1\n"
				<< a1.x + (a2.x - a1.x) * x << ' '
				<< a1.y + (a2.y - a1.y) * x;
		}
		else {
			std::cout << '0';
		}
		return 0;
	}
	else {
		if ((b1 - a1).cross(a2 - a1) != 0) {
			std::cout << '0';
			return 0;
		}

		if (a1.x > a2.x)
			std::swap(a1, a2);
		if (b1.x > b2.x)
			std::swap(b1, b2);

		if (b2.x < a1.x || a2.x < b1.x) {
			std::cout << '0';
			return 0;
		}

		if (a1.y > a2.y)
			std::swap(a1, a2);
		if (b1.y > b2.y)
			std::swap(b1, b2);
		if (b2.y < a1.y || a2.y < b1.y) {
			std::cout << '0';
			return 0;
		}
		std::cout << "1\n";

		if (a1 == b2)
			std::cout << a1.x << ' ' << a1.y;
		if (b1 == a2)
			std::cout << a2.x << ' ' << a2.y;
	}
}
