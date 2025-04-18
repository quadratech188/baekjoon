#include "modules/LazySegmentTree.h"
#include <stdio.h>
#include <vector>

struct Action {
	int type;
	long long int value;
};

struct Data {
	bool reset;
	long long int value;
	long long int add;
	long long int divide;

	void toZero() {
		this->reset = true;
		this->value = 0;
		this->add = 0;
		this->divide = 1;
	}

	void resolve(Data& child1, Data& child2) {
		if (this->reset) {
			child1.toZero();
			child2.toZero();
		}

		child1.update({0, this->add});
		child1.update({1, this->divide});

		child2.update({0, this->add});
		child2.update({1, this->divide});

		this->reset = false;
		this->value = this->calculate();
		this->add = 0;
		this->divide = 1;
	}

	void update(Action action) {
		switch(action.type) {
			case 0:
				this->add += this->divide * action.value;
				break;

			case 1:
				if (this->calculate() < action.value) {
					this->toZero();
					break;
				}
				this->divide *= action.value;
		}
	}
	long long int calculate() const {
		return (this->value + this->add) / this->divide;
	}

	Data operator+(const Data& other) const {
		return {
			.reset = false,
			.value = std::max(this->calculate(), other.calculate()),
			.add = 0,
			.divide = 1
		};
	}
};

int main() {
	int n, q;
	scanf("%d %d", &n, &q);

	std::vector<Data> values;
	values.reserve(n);

	for (int i = 0; i < n; i++) {
		int value;
		scanf("%d", &value);
		values.push_back({
				.value = value,
				.add = 0, 
				.divide = 1,
				});
	}

	LazySegmentTree<Data, Action> segTree(values);

	for (int i = 0; i < q; i++) {
		int t, l, r, x;
		scanf("%d %d %d %d", &t, &l, &r, &x);

		if (t == 2) {
			printf("%lld\n", segTree.query({l, r + 1}).calculate());
		}
		else {
			segTree.update({l, r + 1}, {
					.type = t,
					.value = x
					});
		}
	}
}
