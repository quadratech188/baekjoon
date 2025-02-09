#include "modules/Compress.h"
#include "modules/FastIO.h"
#include "modules/LazySegmentTree.h"
#include <algorithm>
#include <iostream>

enum Type {
	BEGIN,
	END
};

struct Boundary {
	Boundary(int column, int row_begin, int row_end, Type type):
		column(column), row_begin(row_begin), row_end(row_end), type(type) {}
	int column;
	int row_begin, row_end;
	Type type;

	bool operator<(const Boundary& other) {
		return column < other.column;
	}
};

struct Lazy {
	Lazy(): count(0), sum(0), length(1) {}
	Lazy(int count, int sum, int length):
		count(count), sum(sum), length(length) {}

	int count;
	int sum;
	int length;

	void update(int delta) {
		count += delta;
		sum += length * delta;
	}

	Lazy operator+(const Lazy& other) {
		return Lazy(
				0,
				sum + other.sum,
				length + other.length
				);
	}

	void resolve(Lazy& left, Lazy& right) {
		left.update(count);
		right.update(count);
		count = 0;
	}
};

int main() {
	FastIO();
	int n;
	std::cin >> n;
	std::vector<Boundary> data;
	data.reserve(n);

	std::vector<int> rows;
	rows.reserve(2 * n);

	for (int i = 0; i < n; i++) {
		int x1, x2, y1, y2;
		std::cin >> x1 >> x2 >> y1 >> y2;

		rows.push_back(y1);
		rows.push_back(y2);

		data.emplace_back(x1, y1, y2, BEGIN);
		data.emplace_back(x2, y1, y2, END);
	}

	std::sort(rows.begin(), rows.end());

	Compress<int> compressed_rows(rows);

	LazySegmentTree<Lazy> tree(compressed_rows.size());

	std::sort(data.begin(), data.end());

	int prev_column = 0;
	int sum = 0;

	for (Boundary& boundary: data) {
		sum += (boundary.column - prev_column) * tree.root().count;

		int delta = boundary.type == BEGIN? 1: -1;
		tree.update(Segment(
				compressed_rows.compress(boundary.row_begin),
				compressed_rows.compress(boundary.row_end)
				),
				[delta](Lazy& lazy) {
				lazy.update(delta);
				});
	}
	std::cout << sum;
}
