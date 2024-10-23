#include "modules/LazySegTree.h"
#include <iostream>
#include <vector>
#define MOD 1000000007

struct Action {
	int type;
	int v;
};

struct Data {
	long long int a;
	long long int b;
	long long int value;
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
		this->a %= MOD;
		this->b %= MOD;
	}
	void resolve(Data& child1, Data& child2, Segment segment) {
		child1.a *= this->a;
		
		child1.b *= this->a;
		child1.b += this->b;

		child2.a *= this->a;
		
		child2.b *= this->a;
		child2.b += this->b;

		child1.a %= MOD;
		child1.b %= MOD;
		child2.a %= MOD;
		child2.b %= MOD;

		*this = this->resolved(segment);
	}
	Data resolved(Segment segment) {
		return {
			.a = 1,
			.b = 0,
			.value = (this->a * this->value + this->b * segment.size()) % MOD
		};
	}
};

std::vector<int> values;

Data combine_func(Data child1, Segment segment1, Data child2, Segment segment2) {
	return {
		.a = 1,
		.b = 0,
		.value = (child1.a * child1.value + child1.b * segment1.size() + child2.a * child2.value + child2.b * segment2.size()) % MOD
	};
}

Data value_func(size_t index) {
	return {
		.a = 1,
		.b = 0,
		.value = values[index]
	};
}

int main() {
	size_t n;
	std::cin >> n;

	values = std::vector<int>();

	for (int i = 0; i < n; i++) {
		int value;
		std::cin >> value;
		values.push_back(value % MOD);
	}

	LazySegNode<Data, Action> root({0, n}, combine_func, value_func);

	int m;
	std::cin >> m;

	for (int i = 0; i < m; i++) {
		int a, x, y, v;
		std::cin >> a;
		if (a != 4) {
			std::cin >> x >> y >> v;
			Action action = {a, v % MOD};

			root.update({x - 1, y}, action);
		}
		else {
			std::cin >> x >> y;
			
			Data data = root.query({x - 1, y});

			std::cout << (data.a * data.value + data.b) % MOD << std::endl;
		}
	}
}
