#include "Matrix.h"
#include <functional>
#include <cmath>

struct SparseTable {
	Matrix<size_t> values;

	SparseTable(): values(Matrix<size_t>()) {}

	template <typename Callable>
	SparseTable(size_t n, int iterations, Callable func) {
		int sum = 0;

		size_t depth = 1;
		while (true) {
			sum += (1 << (depth - 1));
			if (sum >= iterations) break;
			depth ++;
		}

		values = Matrix<size_t>(n, depth);

		for (size_t i = 0; i < n; i++) {
			values(i, 0) = func(i);
		}

		for (size_t row = 1; row < depth; row++) {
			for (size_t i = 0; i < n; i++) {
				size_t intermediate = values(i, row - 1);
				values(i, row) = values(intermediate, row - 1);
			}
		}
	}

	size_t after(size_t index, int iterations) {
		int i = 0;
		size_t result = index;

		for (size_t i = 0; iterations > 0; i++) {
			if (iterations & 1) {
				result = values(result, i);
			}
			iterations >>= 1;
		}

		return result;
	}

	size_t follow_until_same(size_t a, size_t b) {
		if (a == b) return a;
		for (int i = values.size().y - 1; i >= 0; i--) {
			if (values(a, i) != values(b, i)) {
				a = values(a, i);
				b = values(b, i);
			}
		}
		return values(a, 0);
	}
};
