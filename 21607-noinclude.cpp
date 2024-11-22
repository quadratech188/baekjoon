#include <cmath>
#include <cstddef>
#include <cstdio>
#include <functional>
#include <iostream>
#include <vector>

struct Segment {
	size_t start;
	size_t end;
	size_t size() {
		return end - start;
	}
	inline size_t center() {
		return (start + end) / 2;
	}
	inline Segment left() {
		return {start, center()};
	}
	inline Segment right() {
		return {center(), end};
	}
};

template<typename VAL, typename ACTION>
class LazySegmentTree {
	std::vector<VAL> values;
	size_t length;

	void init(Segment currentSegment, size_t currentIndex, VAL defaultValue) {
		if (currentSegment.size() == 1) {
			this->values[currentIndex] = defaultValue;
			return;
		}

		size_t leftIndex = currentIndex * 2 + 1;
		size_t rightIndex = currentIndex * 2 + 2;

		init(currentSegment.left(), leftIndex, values);
		init(currentSegment.right(), rightIndex, values);

		this->values[currentIndex] = this->values[leftIndex] + this->values[rightIndex];
	}

	void init(Segment currentSegment, size_t currentIndex, std::vector<VAL>& values) {
		if (currentSegment.size() == 1) {
			this->values[currentIndex] = values[currentSegment.start];
			return;
		}

		size_t leftIndex = currentIndex * 2 + 1;
		size_t rightIndex = currentIndex * 2 + 2;

		init(currentSegment.left(), leftIndex, values);
		init(currentSegment.right(), rightIndex, values);

		this->values[currentIndex] = this->values[leftIndex] + this->values[rightIndex];
	}

	VAL query(Segment segment, Segment currentSegment, size_t currentIndex) {
		if (segment.start <= currentSegment.start && currentSegment.end <= segment.end) {
			return this->values[currentIndex];
		}

		size_t leftIndex = currentIndex * 2 + 1;
		size_t rightIndex = currentIndex * 2 + 2;

		this->values[currentIndex].resolve(this->values[leftIndex], this->values[rightIndex]);

		if (currentSegment.center() <= segment.start) {
			return query(segment, currentSegment.right(), rightIndex);
		}
		if (segment.end <= currentSegment.center()) {
			return query(segment, currentSegment.left(), leftIndex);
		}
		return query(segment, currentSegment.left(), leftIndex)
			+ query(segment, currentSegment.right(), rightIndex);
	}

	void update(Segment segment, ACTION action, Segment currentSegment, size_t currentIndex) {
		if (segment.start <= currentSegment.start && currentSegment.end <= segment.end) {
			this->values[currentIndex].update(action);
			return;
		}

		size_t leftIndex = currentIndex * 2 + 1;
		size_t rightIndex = currentIndex * 2 + 2;

		this->values[currentIndex].resolve(this->values[leftIndex], this->values[rightIndex]);

		if (segment.start < currentSegment.center()) {
			update(segment, action, currentSegment.left(), leftIndex);
		}
		if (currentSegment.center() < segment.end) {
			update(segment, action, currentSegment.right(), rightIndex);
		}

		this->values[currentIndex] = this->values[leftIndex] + this->values[rightIndex];
	}

public:
	LazySegmentTree(std::vector<VAL>& values) {
		this->length = values.size();
		this->values.resize(this->length * 4);
		init({0, this->length}, 0, values);
	}
	
	LazySegmentTree(size_t n, VAL defaultValue) {
		this->length = n;
		this->values.resize(this->length * 4);
		init({0, this->length}, 0, defaultValue);
	}

	inline VAL query(Segment segment) {
		return query(segment, {0, this->length}, 0);
	}

	inline void update(Segment segment, ACTION action) {
		update(segment, action, {0, this->length}, 0);
	}
	inline VAL root() {
		return this->values[0];
	}
};

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

template<typename TYPE, int MOD>
struct ModInt{
	TYPE value;

	inline ModInt() {
		this->value = 0;
	}

	inline ModInt(const TYPE other) {
		this->value = other % MOD;
		if (value < 0) value += MOD;
	}
	
	inline ModInt operator=(const TYPE other) {
		this->value = other % MOD;
		return *this;
	}

	inline ModInt operator+(const ModInt& other) const {
		return {(this->value + other.value) % MOD};
	}

	inline ModInt operator+=(const ModInt& other) {
		this->value = (this->value + other.value) % MOD;
		return *this;
	}

	inline ModInt operator*(const ModInt& other) const {
		return {(this->value * other.value) % MOD};
	}

	inline ModInt operator*(const TYPE other) const {
		return {(this->value * other) % MOD};
	}

	inline ModInt operator*=(const ModInt& other) {
		this->value = (this->value * other.value) % MOD;

		return *this;
	}

	inline ModInt operator-(const ModInt& other) {
		return {(this->value - other.value + MOD) % MOD};
	}

	inline operator int() const {
		return this->value;
	}
};

namespace Input {
	template<typename T>
	inline std::vector<T> inlineToVec(size_t n) {
		std::vector<T> result;
		result.reserve(n);
		for (int i = 0; i < n; i++) {
			T value;
			std::cin >> value;
			result.push_back(value);
		}
		return result;
	}
}

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

typedef ModInt<long long int, 100003> mInt;

SparseTable fTable(100003, 500000, [](mInt index) {
		return (mInt)2 * index * index - (mInt)1;
		});

SparseTable gTable(100003, 500000, [](mInt index) {
		return (mInt)4 * index * index * index - (mInt)3 * index;
		});

enum Function {
	F,
	G
};

struct Data {
	int fTimes;
	int gTimes;

	void update(Function func) {
		if (func == F) {
			fTimes ++;
		}
		else {
			gTimes ++;
		}
	}

	void resolve(Data& child1, Data& child2) {
		child1.fTimes += this->fTimes;
		child1.gTimes += this->gTimes;
		child2.fTimes += this->fTimes;
		child2.gTimes += this->gTimes;

		this->fTimes = 0;
		this->gTimes = 0;
	}

	Data operator+(const Data& other) {
		return {
			.fTimes = 0,
			.gTimes = 0
		};
	}
};

int main() {
	FastIO();
	int n, q;
	std::cin >> n >> q;

	std::vector<int> values = Input::inlineToVec<int>(n);

	LazySegmentTree<Data, Function> tree(n, {
			.fTimes = 0,
			.gTimes = 0
			});

	for (int i = 0; i < q; i++) {
		size_t a, b, c;
		std::cin >> a;
		switch(a) {
			case 1:
				std::cin >> b >> c;
				tree.update({b - 1, c}, F);
				break;
			case 2:
				std::cin >> b >> c;
				tree.update({b - 1, c}, G);
				break;
			case 3:
				std::cin >> b;
				Data data = tree.query({b - 1, b});
				std::cout << gTable.after(fTable.after(values[b - 1], data.fTimes), data.gTimes) << '\n';
		}
	}
}
