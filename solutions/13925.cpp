#include "modules/LazySegmentTree.h"
#include "modules/ModInt.h"
#include <stdio.h>
#include <vector>

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
