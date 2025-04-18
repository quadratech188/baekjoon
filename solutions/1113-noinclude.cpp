#include <array>
#include <cstdio>
#include <functional>
#include <iostream>
#include <queue>
#include <span>
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

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

int main() {
	FastIO();
	size_t n, m;
	std::cin >> n >> m;
	int maxHeight = 0;

	Tensor<2, int> landscape({m + 2, n + 2}, [n, m, &maxHeight](auto index, int& value) {
			if (index[0] == 0 || index[0] == m + 1) {
				value = 0;
				return;
			}
			if (index[1] == 0 || index[1] == n + 1) {
				value = 0;
				return;
			}

			char temp;
			std::cin >> temp;
			value = temp - '0';
			maxHeight = std::max(value, maxHeight);
			});

	Tensor<2, int> waterHeights({m + 2, n + 2}, [maxHeight](auto index, int& value) {
			value = maxHeight;
			});

	Tensor<2, int> visited({m + 2, n + 2}, [](auto index, int& value) {
			value = false;
			});

	std::queue<std::array<size_t, 2>> queue;

	for (int waterHeight = maxHeight; waterHeight >= 0; waterHeight --) {
		visited.forEachRow([](auto index, int& value) {value = false;});
		queue.push({0, 0});

		while (!queue.empty()) {
			auto pos = queue.front();
			queue.pop();

			waterHeights[pos] = waterHeight;

			waterHeights.forEachNeighbor(pos, [&landscape, &waterHeight, &queue, &visited](auto index, int& height) {
					if (landscape[index] <= waterHeight && !visited[index]) {
						visited[index] = true;
						queue.push(index);
					}
					});
		}
	}

	int sum = 0;

	for (const auto& value: waterHeights.forEachRow()) {
		sum += value.second - landscape[value.first];
	}

	std::cout << sum;
}
