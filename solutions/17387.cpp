#include "../modules/Vec2.h"
#include "../modules/FastIO2.h"

bool check(int64_t numerator, int64_t denominator) {
	if (denominator == 0) return true;

	if (denominator < 0)
		return denominator <= numerator && numerator <= 0;
	else
	 	return 0 <= numerator && numerator <= denominator;
}

int main() {
	Int2 a1, a2, b1, b2;
	Fast::cin >> a1 >> a2 >> b1 >> b2;

	bool result = true;

	// a1 + (a2 - a1) x = b1 + (b2 - b1) y
	
	// (a2 - a1) cross (b2 - b1) x = (b1 - a1) cross (b2 - b1)
	
	result &= check((b1 - a1).cross(b2 - b1), (a2 - a1).cross(b2 - b1));

	// (a1 - b1) cross (a2 - a1) = (b2 - b1) cross (a2 - a1) y
	result &= check((a1 - b1).cross(a2 - a1), (b2 - b1).cross(a2 - a1));

	if ((a2 - a1).cross(b2 - b1) == 0) {
		result &= (b1 - a1).cross(a2 - a1) == 0;
		result &= (b1 - a2).cross(a2 - a1) == 0;
		result &= (b2 - a1).cross(a2 - a1) == 0;
		result &= (b2 - a2).cross(a2 - a1) == 0;


		if (a1.x > a2.x) std::swap(a1, a2);
		if (b1.x > b2.x) std::swap(b1, b2);
		result &= a1.x <= b2.x && b1.x <= a2.x;

		if (a1.y > a2.y) std::swap(a1, a2);
		if (b1.y > b2.y) std::swap(b1, b2);
		result &= a1.y <= b2.y && b1.y <= a2.y;
	}

	std::cout << result;
}
