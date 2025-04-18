#include <iostream>
#include <utility>
#include "modules/Matrix.h"

int main() {
	int n, m;
	std::cin >> n >> m;

	Matrix<int> lights(m, n);
	std::cin >> lights;
	Matrix<int> result(m, n);
	std::cin >> result;
	std::vector<int> rowCount(n);
	Matrix<int> columnCount(m, m);

	int count = 0;

	for(int row = 0; row < lights.size().y; row++) {
		int lightCount = 0;
		int resultCount = 0;
		for (int col = 0; col < lights.size().x; col++) {
			lightCount += lights[col][row];
			resultCount += result[col][row];
		}
		if (lightCount == resultCount)
			continue;
		if (lightCount == lights.size().x - resultCount) {
			for (int col = 0; col < lights.size().x; col++) {
				lights[col][row] = 1 - lights[col][row];
			}
			std::cout << "0 " << row + 1 << '\n';
			count ++;
			continue;
		}
		std::cout << -1;
		return 0;
	}
}
