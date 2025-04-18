#include "modules/Compress.h"
#include "modules/FastIO.h"
#include "modules/LazySegmentTree.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <ranges>

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

struct LazyColor {
	int length;
	int count;
	int layers;

	LazyColor(int length = 0):
		length(length), count(0), layers(0) {}

	LazyColor(int length, int count, int layers):
		length(length), count(count), layers(layers) {}

	int value() {
		if (layers == 0) return count;
		return length;
	}

	LazyColor operator+(LazyColor& other)  {
		int delta = std::min(layers, other.layers);
		layers -= delta;
		other.layers -= delta;

		return LazyColor(
				length + other.length,
				value() + other.value(),
				delta
				);
	}

	bool update(int delta) {
		if (layers + delta >= 0) {
			// All good
			layers += delta;
			// Stop here
			return false;
		}
		// resolve() will update the children with layers, we don't do anything here
		return true;
	}

	void resolve(LazyColor& left, LazyColor& right) {
		left.update(layers);
		right.update(layers);
	}
};

int main() {
	FastIO();
	int n;
	std::cin >> n;
	std::vector<Boundary> data;
	data.reserve(2 * n);


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


	LazySegmentTree<LazyColor> tree(
			std::views::iota((size_t)0, compressed_rows.size() - 1)
			| std::views::transform([&compressed_rows](size_t index) {
				return compressed_rows.decompress(index + 1)
				- compressed_rows.decompress(index);
				})
			);

	std::sort(data.begin(), data.end());

	int prev_column = 0;
	long long int sum = 0;

	for (Boundary& boundary: data) {
		sum += static_cast<long long int>(boundary.column - prev_column) * tree.root().value();

		int delta = boundary.type == BEGIN? 1: -1;
		tree.update(
				compressed_rows.compress(boundary.row_begin),
				compressed_rows.compress(boundary.row_end),
				[delta](LazyColor& val) {
				return val.update(delta);
				});
		
		prev_column = boundary.column;
	}
	std::cout << sum;
}
