#include <array>
#include <span>
#include <iostream>
#include <functional>
#include <vector>

template <size_t ORDER, typename T>
struct Tensor {
	typedef std::array<size_t, ORDER> INDEX;
	typedef std::function<void(const INDEX&, T&)> FUNC;

	INDEX size;

	std::vector<T> values;

	Tensor(INDEX size, FUNC func) {
		this->size = size;

		size_t valueSize = 1;
		for (auto i: size) {
			valueSize *= i;
		}
		this->values.resize(valueSize);
		
		forEachRow(func);
	}

	T& operator[] (INDEX indices) {
		size_t valueIndex = 0;

		for (int i = 0; i < ORDER; i++) {
			valueIndex = this->size[i] * valueIndex + indices[i];
		}
		return this->values[valueIndex];
	}

	class RowIterator {
	public:
		RowIterator(Tensor& tensor): tensor(tensor), index({0}), count(0) {}
		RowIterator(Tensor& tensor, int count): tensor(tensor), count(count) {}

		std::pair<INDEX, T&> operator*() {
			return {index, tensor[index]};
		}

		INDEX index;

		RowIterator& operator++() {
			index[0] += 1;

			if (index[0] == tensor.size[0]) {
				for (size_t i = 0; index[i] == tensor.size[i]; i++) {
					index[i] = 0;
					index[i + 1] ++;
				}
			}

			count += 1;

			return *this;
		}

		bool operator!=(RowIterator& other) const {
			return this->count != other.count;
		}

	private:
		Tensor& tensor;
		int count;
	};

	class RowTensor {
	public:
		RowTensor(Tensor& tensor): tensor(tensor) {}

		RowIterator begin() {
			return RowIterator(tensor);
		}
	
		RowIterator end() {
			return RowIterator(tensor, tensor.values.size());
		}
	private:
		Tensor& tensor;
	};

	RowTensor forEachRow() {
		return RowTensor(*this);
	}

	void forEachRow(FUNC func) {
		INDEX index = {0};

		for (size_t _ = 0; _ < this->values.size(); _++) {
			func(index, (*this)[index]);

			index[0] += 1;

			if (index[0] == this->size[0]) {
				for (size_t i = 0; index[i] == this->size[i]; i++) {
					index[i] = 0;
					index[i + 1] ++;
				}
			}
		}
	}

	void forEachNeighbor(INDEX index, FUNC func) {
		for (size_t i = 0; i < ORDER; i++) {
			if (index[i] > 0) {
				index[i] -= 1;
				func(index, (*this)[index]);
				index[i] += 1;
			}
			if (index[i] < this->size[i] - 1) {
				index[i] += 1;
				func(index, (*this)[index]);
				index[i] -= 1;
			}
		}
	}
};
