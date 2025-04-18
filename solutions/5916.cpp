#include "modules/LazySegTree.h"
#include <iostream>
#include <vector>
#define MOD 1000000007

struct Data {
	int value;
	int lazy;
	void update(bool action) {
		this->value += 1;
	}
	void resolve(Data& child1, Data& child2, Segment segment) {
		
		child1.lazy = this->lazy;
		child2.lazy = this->lazy;
		
		*this = this->resolved(segment);
	}
	Data resolved(Segment segment) {
		return {
			.value = this->value + this->lazy * segment.size(),
			.lazy = 0
		};
	}
};

std::vector<int> values;

Data combine_func(Data child1, Data child2) {
	return {
		.value = child1.value + child1.lazy +  child2.value + child2.lazy,
		.lazy = 0
	};
}

Data value_func(size_t index) {
	return {
		.value = values[index],
		lazy = 0
	};
}

int main() {
	size_t n;
	std::cin >> n;

	values = std::vector<int>(n - 1, 0);

	LazySegNode<Data, Action> root({0, n - 1}, combine_func, value_func);

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
