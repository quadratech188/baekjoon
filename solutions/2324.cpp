#include "modules/Matrix.h"
#include "modules/FastIO.h"

int main() {
	FastIO();

	int n, m;
	std::cin >> n >> m;

	Matrix<char> values(m, n);

	std::cin >> values;

}
