#include <iostream>

#include "modules/LazySegmentTree.h"
#include "modules/SparseTable.h"
#include "modules/ModInt.h"
#include "modules/Input.h"
#include "modules/FastIO.h"

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
