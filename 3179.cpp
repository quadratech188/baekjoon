#include "modules/Vec2.h"
#include "modules/Input.h"
#include "modules/Matrix.h"
#include <iostream>
#include <vector>

int main() {
	int n;
	std::cin >> n;

	std::vector<Int2> points = Input::inlineToVec<Int2>(n);

	Matrix<int> counts(n, n);

	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			Int2 a = points[i];
			Int2 b = points[j];

			for (Int2& point: points) {
				if ((b - a).cross(point - a) >= 0) counts.at(i, j) ++;
			}
		}
	}


}
