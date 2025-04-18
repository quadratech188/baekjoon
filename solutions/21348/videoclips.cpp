#include <cmath>
#include <functional>
#include <vector>

template <typename T>
struct Matrix {
	std::vector<T> values;

	int sizeX, sizeY;

	Matrix(int x, int y, int defaultValue = T()) {
		values = std::vector<T>(x * y);
		sizeX = x;
		sizeY = y;
	}

	T& at(int x, int y) {
		return this->values[x * sizeY + y];
	}
	Matrix(): sizeX(0), sizeY(0), values(std::vector<T>()) {}
};

struct SparseTable {
	Matrix<size_t> values;

	SparseTable(): values(Matrix<size_t>()) {}

	SparseTable(size_t n, int iterations, std::function<int(int)> func) {
		int sum = 0;

		size_t depth = 1;
		while (true) {
			sum += (1 << (depth - 1));
			if (sum >= iterations) break;
			depth ++;
		}

		values = Matrix<size_t>(n, depth);

		for (size_t i = 0; i < n; i++) {
			values.at(i, 0) = func(i);
		}

		for (size_t row = 1; row < depth; row++) {
			for (size_t i = 0; i < n; i++) {
				size_t intermediate = values.at(i, row - 1);
				values.at(i, row) = values.at(intermediate, row - 1);
			}
		}
	}

	size_t after(size_t index, int iterations) {
		int i = 0;
		size_t result = index;

		for (size_t i = 0; iterations > 0; i++) {
			if (iterations & 1) {
				result = values.at(result, i);
			}
			iterations >>= 1;
		}

		return result;
	}
};

#include "videoclips.h"

int k, m;
SparseTable table;

void videos(int K, int M, int S[]) {
	table = SparseTable(K, M, [&S](int index) {
			return S[index];
			});
	m = M;
}

int clip(int I) {
	return table.after(I, m - 1);
}
