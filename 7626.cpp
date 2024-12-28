#include "modules/LazySegmentTree.h"
#include <iostream>
#include <set>

struct Data {
	int column;
	int row_begin, row_end;
};

int main() {
	int n;
	std::cin >> n;
	std::vector<Data> data;

	std::set<int> rows;
	std::set<int> columns;

	for (int i = 0; i < n; i++) {
		int x1, x2, y1, y2;
		std::cin >> x1 >> x2 >> y1 >> y2;

		rows.insert(y1);
		rows.insert(y2);
		columns.insert(x1);
		columns.insert(x2);
	}
}
