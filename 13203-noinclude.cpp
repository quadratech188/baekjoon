#include <cstddef>
#include <cstdio>
#include <float.h>
#include <iostream>
#include <unordered_map>
#include <utility>
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
class SegmentTree {
	std::vector<VAL> values;
	size_t length;

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
	void init(Segment currentSegment, size_t currentIndex, VAL& defaultValue) {
		if (currentSegment.size() == 1) {
			this->values[currentIndex] = defaultValue;
			return;
		}

		size_t leftIndex = currentIndex * 2 + 1;
		size_t rightIndex = currentIndex * 2 + 2;

		init(currentSegment.left(), leftIndex, defaultValue);
		init(currentSegment.right(), rightIndex, defaultValue);

		this->values[currentIndex] = this->values[leftIndex] + this->values[rightIndex];
	}

	VAL query(Segment segment, Segment currentSegment, size_t currentIndex) {
		if (segment.start <= currentSegment.start && currentSegment.end <= segment.end) {
			return this->values[currentIndex];
		}

		size_t leftIndex = currentIndex * 2 + 1;
		size_t rightIndex = currentIndex * 2 + 2;

		if (currentSegment.center() <= segment.start) {
			return query(segment, currentSegment.right(), rightIndex);
		}
		if (segment.end <= currentSegment.center()) {
			return query(segment, currentSegment.left(), leftIndex);
		}
		return query(segment, currentSegment.left(), leftIndex) + query(segment, currentSegment.right(), rightIndex);
	}

	void update(size_t index, ACTION action, Segment currentSegment, size_t currentIndex) {
		if (currentSegment.size() == 1) {
			this->values[currentIndex].update(action);
			return;
		}

		size_t leftIndex = currentIndex * 2 + 1;
		size_t rightIndex = currentIndex * 2 + 2;

		if (index < currentSegment.center()) {
			update(index, action, currentSegment.left(), leftIndex);
		}
		else {
			update(index, action, currentSegment.right(), rightIndex);
		}

		this->values[currentIndex] = this->values[leftIndex] + this->values[rightIndex];
	}

public:
	SegmentTree(std::vector<VAL>& values) {
		this->length = values.size();
		this->values = std::vector<VAL>(4 * this->length);
		init({0, this->length}, 0, values);
	}
	SegmentTree(size_t size, VAL defaultValue) {
		this->length = size;
		this->values = std::vector<VAL>(4 * size);
		init({0, size}, 0, defaultValue);
	}
	inline VAL query(Segment segment) {
		return query(segment, {0, this->length}, 0);
	}
	inline void update(size_t index, ACTION action) {
		update(index, action, {0, this->length}, 0);
	}
	VAL root() {
		return this->values[0];
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

	inline operator int() const {
		return this->value;
	}
};

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

typedef ModInt<int, 1000000007> INT;

struct Data {
	INT values[6][6];
	char chr;

	Data() {
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 6; j++) {
				this->values[i][j] = 0;
			}
		}
	}

	Data(char ch, std::string& name) {
		this->chr = ch;
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 6; j++) {
				if (i == j) {
					this->values[i][j] = 1;
				}
				else {
					this->values[i][j] = 0;
				}
			}
		}
		for (int i = 0; i < name.size(); i++) {
			if (name[i] == ch) {
				this->values[i][i + 1] = 1;
			}
		}
	}

	Data operator+(const Data& other) {
		Data result;
		for (int a = 0; a < 6; a++) {
			for (int b = a; b < 6; b++) {
				for (int c = b; c < 6; c++) {
					result.values[a][c] += this->values[a][b] * other.values[b][c];
				}
			}
		}
		return result;
	}

	void update(std::pair<char, std::string&> action) {
		std::string& name = action.second;
		char ch = action.first;

		for (int i = 0; i < name.size(); i++) {
			if (name[i] == this->chr)
				this->values[i][i + 1] = 0;
			if (name[i] == ch)
				this->values[i][i + 1] = 1;
		}
		this->chr = ch;
	}
};

void loop() {
	int n, m, q;
	std::cin >> n >> m >> q;

	std::string str;
	std::cin >> str;
	std::string name;
	std::cin >> name;

	std::vector<Data> dataVec;
	dataVec.reserve(n);
	for (char ch: str) {
		dataVec.emplace_back(ch, name);
	}

	SegmentTree<Data, std::pair<char, std::string&>> data(dataVec);

	std::cout << data.root().values[0][m] << std::endl;

	for (int i = 0; i < q; i++) {
		int a, b;
		std::string segment;
		std::cin >> a >> b >> segment;

		for (int i = 0; i < b - a + 1; i++) {
			data.update(a - 1 + i, {segment[i], name});
		}
		std::cout << data.root().values[0][m] << std::endl;
	}
}

int main() {
	FastIO();
	int t;
	std::cin >> t;
	for (int i = 0; i < t; i++) {
		loop();
	}
}
