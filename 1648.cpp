#include "modules/Matrix.h"
#include "modules/ModInt.h"
#include <optional>

Matrix<std::optional<mInt>> cache;
int n, m;

int main() {
	std::cin >> n >> m;

	cache = Matrix<std::optional<mInt>>(1 << n, m);
}
