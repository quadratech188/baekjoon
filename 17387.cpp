#include "modules/Vec2.h"

typedef Vec2<double, double> Double2;

bool intersects(Double2 v1, Double2 v2, Double2 v3, Double2 v4) {
	double ratio1 = (v2 - v1).cross(v3 - v1);
	double ratio2 = (v2 - v1).cross(v4 - v1);

	if (ratio1 == 0 && ratio2 == 0) {
		return 0 <= (v3 - v1).dot(v2 - v1) &&
			(v3 - v1).dot(v2 - v1) <= (v2 - v1).size2() ||
			0 <= (v4 - v1).dot(v2 - v1) &&
			(v4 - v1).dot(v2 - v1) <= (v2 - v1).size2() ||
			intersects(v3, v4, v1, v2);
	}

	if (ratio1 * ratio2 > 0) return false;

	Double2 intersection = (v3 * ratio2 - v4 * ratio1) / (ratio2 - ratio1);

	return 0 <= (intersection - v1).dot(v2 - v1) &&
		(intersection - v1).dot(v2 - v1) <= (v2 - v1).size2();
}

int main() {
	Double2 v1, v2, v3, v4;
	std::cin >> v1 >> v2 >> v3 >> v4;

	std::cout << intersects(v1, v2, v3, v4);
}
