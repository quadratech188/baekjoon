#pragma once

#include <functional>
#include <iostream>
#include <valarray>

#include "Vec2.h"
#include "Bounds2.h"

template <typename T>
class Matrix {
public:

	Matrix(int columns, int rows, T defaultValue = T()):
		_size(columns, rows), values(columns * rows, defaultValue) {}

	Matrix(): _size(Int2::zero()), values() {}

	T& operator()(int column, int row) {
		return this->values[row * _size.x + column];
	}

	const T& operator()(int column, int row) const {
		return this->values[row * _size.x + column];
	}

	T& operator[](Int2 index) {
		return *this(index.x, index.y);
	}

	bool operator==(const Matrix<T>& other) const {
		return _size == other._size && values == other.values;
	}
	
	bool operator<(const Matrix<T>& other) const {
		if (_size != other._size) return _size < other._size;

		return values < other.values;
	}

	Matrix operator*(const Matrix& other) {
		Matrix result(other._size.x, _size.y);

		for (Int2 index: result.bounds()) {
			for (int depth = 0; depth < _size.x; depth++) {
				result[index] += *this(depth, index.y) * *this(index.x, depth);
			}
		}

		return result;
	}

	Int2 size() {
		return _size;
	}

	Range2 bounds() {
		return Range2(Int2::zero(), _size);
	}

	int rawIndex(Int2 index) const {
		return index.y * _size.x + index.x;
	}

private:
	std::valarray<T> values;

	Int2 _size;
};

template<typename T>
std::istream& operator>>(std::istream& input, Matrix<T>& matrix) {
	for (Int2 index: matrix.bounds())
		input >> matrix[index];

	return input;
}
