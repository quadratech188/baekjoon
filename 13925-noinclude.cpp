#include <cstddef>
#include <stdio.h>
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

	void init(Segment currentSegment, size_t currentIndex, std::vector<VAL> values) {
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
	LazySegmentTree(std::vector<VAL> values) {
		this->length = values.size();
		this->values.resize(this->length * 2);
		init({0, this->length}, 0, values);
	}
	inline VAL query(Segment segment) {
		return query(segment, {0, this->length}, 0);
	}

	inline void update(Segment segment, ACTION action) {
		update(segment, action, {0, this->length}, 0);
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

typedef ModInt<long long int, 1000000007> INT;

struct Action {
	int type;
	INT v;
};

struct Data {
	INT a;
	INT b;
	INT value;
	size_t length;
	
	inline INT resolvedValue() const {
		return this->a * this->value + this->b * (INT)(int)(this->length);
	}

	Data operator+(const Data& other) {
		return {
			.a = 1,
			.b = 0,
			.value = this->resolvedValue() + other.resolvedValue(),
			.length = this->length + other.length
		};
	}

	void update(Action action) {
		switch(action.type) {
			case 1:
				this->b += action.v;
				break;
			case 2:
				this->a *= action.v;
				this->b *= action.v;
				break;
			case 3:
				this->a = 0;
				this->b = action.v;
		}
	}
	void resolve(Data& child1, Data& child2) {
		child1.a *= this->a;
		
		child1.b *= this->a;
		child1.b += this->b;

		child2.a *= this->a;
		
		child2.b *= this->a;
		child2.b += this->b;

		this->value = this->resolvedValue();
		this->a = 1;
		this->b = 0;
	}
};

int main() {
	int n;
	scanf("%d", &n);

	std::vector<Data> values;

	values.reserve(n);

	for (int i = 0; i < n; i++) {
		int value;
		scanf("%d", &value);
		values.push_back({
				.a = 1,
				.b = 0,
				.value = value,
				.length = 1
				});
	}

	LazySegmentTree<Data, Action> root(values);

	int m;
	scanf("%d", &m);

	for (int i = 0; i < m; i++) {
		int a, x, y, v;
		scanf("%d", &a);
		if (a != 4) {
			scanf("%d %d %d", &x, &y, &v);
			Action action = {a, v};

			root.update({x - 1, y}, action);
		}
		else {
			scanf("%d %d", &x, &y);
			
			Data data = root.query({x - 1, y});

			printf("%d\n", (int)data.resolvedValue());
		}
	}
}
