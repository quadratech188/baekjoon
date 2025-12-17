#pragma once
#include <ranges>
#include <vector>
#include "Vec2.h"

template <typename T>
class Matrix {
public:
	Matrix(int i, int j, T const& default_value = {}):
		Matrix({i, j}, default_value) {}

	Matrix(Int2 size, T const& default_value = {}):
		_size(size), _values(size.x * size.y, default_value) {}

	template <std::ranges::sized_range R>
	Matrix(int i, int j, R const& range):
		Matrix({i, j}, range) {}

	template <std::ranges::sized_range R>
	Matrix(Int2 size, R const& range):
		_size(size), _values(range.begin(), range.end()) {}
private:
	Int2 _size;
	std::vector<T> _values;

public:
	Int2 size() {
		return _size;
	}

	T& operator()(int i, int j) {
		return _values[_size.y * i + j];
	}
	T const& operator()(int i, int j) const {
		return _values[_size.y * i + j];
	}

	T& operator[](Int2 i) {
		return (*this)(i.x, i.y);
	}
	T const& operator[](Int2 i) const {
		return (*this)(i.x, i.y);
	}

	bool bounds_check(int i, int j) const {
		return 0 <= i && i < _size.x && 0 <= j && j < _size.y;
	}
	bool bounds_check(Int2 i) const {
		return bounds_check(i.x, i.y);
	}

	auto keys() {
		return std::ranges::iota_view(0, _size.x)
			| std::views::transform([this](int i) {
			return std::ranges::iota_view(0, _size.y)
				| std::views::transform([i](int j) {
				return Int2(i, j);
				});
			})
			| std::views::join;
	}

	auto items() {
		return keys()
			| std::views::transform([this](Int2 i) -> std::pair<Int2, T&> {
			return {i, (*this)[i]};
			});
	}

	Matrix operator+(Matrix const& other) {
		Matrix result(_size);

		for (uint i = 0; i < _values.size(); i++) {
			result._values[i] = _values[i] + other._values[i];
		}
		return result;
	}

	Matrix operator*(Matrix const& other) {
		int depth = _size.y = other._size.x;

		Matrix result(_size.x, other._size.y, 0);

		for (int i = 0; i < result._size.x; i++) {
			for (int j = 0; j < result._size.y; j++) {
				for (int k = 0; k < depth; k++) {
					result(i, j) += (*this)(i, k) * other(k, j);
				}
			}
		}
		return result;
	}

	static Matrix identity(int size) {
		Matrix result(size, size, 0);
		for (int i = 0; i < size; i++) {
			result(i, i) = 1;
		}
		return result;
	}

	Matrix identity() {
		return identity(_size.x);
	}

	template <typename IS>
	friend IS& operator>>(IS& is, Matrix& matrix) {
		for (auto& val: matrix._values)
			is >> val;
		return is;
	}

	template <typename OS>
	friend OS& operator<<(OS& os, Matrix const& matrix) {
		for (int i = 0; i < matrix._size.x; i++) {
			for (int j = 0; j < matrix._size.y; j++) {
				os << matrix(i, j) << ' ';
			}
			os << '\n';
		}
		return os;
	}
};
