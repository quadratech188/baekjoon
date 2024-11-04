#include "modules/LazySegmentTree.h"
#include <stdio.h>
#include <vector>

enum Paint {
	BLACK,
	WHITE,
	MIXED
};

struct Data {
	int wrongWhite;
	int rightWhite;
	int wrongBlack;
	int rightBlack;
	Paint color;

	void update(Paint color) {
		switch(color) {
			case WHITE:
				this->wrongWhite += this->rightBlack;
				this->rightWhite += this->wrongBlack;
				this->wrongBlack = 0;
				this->rightBlack = 0;
				this->color = WHITE;
				break;
			case BLACK:
				this->wrongBlack += this->rightWhite;
				this->rightBlack += this->wrongWhite;
				this->wrongWhite = 0;
				this->rightWhite = 0;
				this->color = BLACK;
				break;
		}
	}
	Data operator+(const Data other) const {
		return {
			.wrongWhite = this->wrongWhite + other.wrongWhite,
			.rightWhite = this->rightWhite + other.rightWhite,
			.wrongBlack = this->wrongBlack + other.wrongBlack,
			.rightBlack = this->rightBlack + other.rightBlack,
			.color = MIXED
		};
	}
	void resolve(Data& child1, Data& child2) {
		if (this->color == MIXED) return;
		child1.update(this->color);
		child2.update(this->color);
		this->color = MIXED;
	}
	int correct() {
		return this->rightWhite + this->rightBlack;
	}
};

int main() {
	int r, c, q;
	scanf("%d %d %d", &r, &c, &q);
	
	std::vector<std::vector<Data>> data(c);

	for (auto& column: data) {
		column.reserve(r);
	}
	
	getchar();

	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			char value;
			scanf("%c", &value);

			data[j].push_back({
					.wrongWhite = 0,
					.rightWhite = 0,
					.wrongBlack = value == '1'? 1: 0,
					.rightBlack = value == '0'? 1: 0
					});
		}
		getchar();
	}
/*
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			printf("%d", data[j][i].wrongBlack);
		}
		printf("\n");
	}
*/
	std::vector<LazySegmentTree<Data, Paint>> segData;

	segData.reserve(c);

	for (auto& column: data) {
		segData.emplace_back(column);
	}

	for (int i = 0; i < q; i++) {
		int r1, r2, c1, c2, x;
		scanf("%d %d %d %d %d", &r1, &r2, &c1, &c2, &x);
		for (int column = c1 - 1; column <= c2 - 1; column++) {
			segData[column].update({r1 - 1, r2}, x == 1? WHITE: BLACK);
		}

		int sum = 0;

		for (auto& column: segData) {
			sum += column.root().correct();
		}
		printf("%d\n", sum);
	}
}
