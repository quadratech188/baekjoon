#include "modules/Matrix.h"
#include "modules/ModInt.h"
#include <iostream>

int main() {
	int d;
	std::cin >> d;

	Matrix<m1e9_7> matrix(8, 8);
	matrix(0, 1) = 1;
	matrix(0, 7) = 1;
}
