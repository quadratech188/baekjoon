#define FASTISTREAM_BUFFER_SIZE 1 << 18

#include "FastIO2.h"
#include "Matrix.h"
#include "ModInt.h"
#include <cstdio>

int main() {
	int t, n, d;
	Fast::cin >> t >> n >> d;

	auto total = Matrix<sm32_1e9_7>::identity(n);
	auto remainder = Matrix<sm32_1e9_7>::identity(n);

	for (int i = 0; i < t; i ++) {
		Matrix<sm32_1e9_7> map(n, n, 0);
		int m;
		Fast::cin >> m;
		for (int _ = 0; _ < m; _ ++) {
			int a, b, c;
			Fast::cin >> a >> b >> c;
			map(a - 1, b - 1) = c;
		}
		total = total * map;
		if (i < d % t) {
			remainder = remainder * map;
		}
	}

	auto result = Math::power(total, d / t, total.identity()) * remainder;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			printf("%d ", result(i, j).val());
		}
		printf("\n");
	}
}
