#include <functional>
#include <vector>

template <size_t DIMENSIONS, typename VAL>
struct Matrix {
	std::vector<Matrix<DIMENSIONS - 1, VAL>> children;

	Matrix(std::vector<size_t> size) {
		children.resize(size[DIMENSIONS - 1], size);
	}

	Matrix(std::vector<size_t> size, std::function<VAL(std::vector<size_t>)> valueFunc) {
		children.resize(size[DIMENSIONS - 1], size, valueFunc);
	}

	inline VAL& operator[](std::vector<size_t> index) {
		return this->children[index[DIMENSIONS - 1]][index];
	}
	inline VAL& operator[](size_t index) {
		return this->children[index];
	}

	void callNeighbors(std::vector<size_t> index, std::function<void(VAL&, std::vector<size_t>)> func) {
		if (index[DIMENSIONS - 1] > 0) {
			index[DIMENSIONS - 1] -= 1;
			func(this[index]);
			index[DIMENSIONS - 1] += 1;
		}
		if (index[DIMENSIONS - 1] < this->children.size() - 1) {
			index[DIMENSIONS - 1] += 1;
			func(this[index]);
			index[DIMENSIONS - 1] -= 1;
		}
		this[index[DIMENSIONS - 1]].callNeighbors(index, func);
	}
};

template <typename VAL>
struct Matrix<1, VAL> {
	std::vector<VAL> values;

	Matrix(std::vector<size_t> size) {
		values.resize(size[0]);
	}

	Matrix(std::vector<size_t> size, std::function<VAL(std::function<size_t>)>) {
		values.reserve(size[0]);

		for (int i = 0; i < size[0]; i++) {
			values[i] = func();
		}
	}

	inline VAL& operator[](std::vector<size_t> index) {
		return this->values[index[0]];
	}
	inline VAL& operator[](size_t index) {
		return this->values[index];
	}
	void callNeighbors(std::vector<size_t> index, std::function<void(VAL&, std::vector<size_t>)> func) {
		if (index[0] > 0) {
			func(this[index[0] - 1]);
		}
		if (index[0] < this->values.size() - 1) {
			func(this[index[0] + 1]);
		}
	}
};
