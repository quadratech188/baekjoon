#include <array>
#include <cstdio>
#include <functional>
#include <iostream>
#include <queue>
#include <span>
#include <utility>
#include <vector>

template <size_t ORDER, typename T>
struct Tensor {
	typedef std::array<size_t, ORDER> INDEX;
	typedef std::function<void(const INDEX&, T&)> FUNC;

	INDEX size;

	std::vector<T> values;

	Tensor(INDEX size, FUNC func, bool rowsFirst = false) {
		this->size = size;

		size_t valueSize = 1;
		for (auto i: size) {
			valueSize *= i;
		}
		this->values.resize(valueSize);
		
		if (rowsFirst) {
			forEachRow(func);
		}
		else {
			forEach(func);
		}
	}

	T& operator[] (INDEX indices) {
		size_t valueIndex = 0;

		for (int i = 0; i < ORDER; i++) {
			valueIndex = this->size[i] * valueIndex + indices[i];
		}
		return this->values[valueIndex];
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

	void forEach(std::function<void(INDEX, T&)> func) {
		//TODO: Implement
	}
};

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

enum TomatoState {
	NONE,
	NOT_RIPE,
	RIPE
};

struct Data {
	int time;
	std::array<size_t, 11> index;
};

int main() {
	FastIO();

	std::array<size_t, 11> size;

	for (int i = 0; i < 11; i++) {
		std::cin >> size[i];
	}

	std::queue<Data> queue;

	int notRipeCount = 0;

	Tensor<11, TomatoState> tomatoes(size, [&notRipeCount, &queue](auto index, TomatoState& value) {
			int type;
			std::cin >> type;
			if (type == 1) {
				value = RIPE;
				queue.push({.time = 0, .index = index});
			}
			else if (type == 0) {
				value = NOT_RIPE;
				notRipeCount ++;
			}
			else {
				value = NONE;
			}
			}, true);

	int time;

	while (!queue.empty()) {
		Data value = queue.front();
		queue.pop();

		time = value.time;

		tomatoes.forEachNeighbor(value.index, [&notRipeCount, &value, &queue](auto index, TomatoState& state) {
				if (state == NOT_RIPE) {
					state = RIPE;
					queue.push({.time = value.time + 1, .index = index});
					notRipeCount --;
				}
				});
	}
		if (notRipeCount != 0) {
			std::cout << -1;
		}
		else {
			std::cout << time;
		}
}
