#include <algorithm>
#include <iostream>
#include <vector>
#include "../modules/LazySegmentTree.h"
#include "../modules/Compress.h"
#include "../modules/FastIO.h"
struct Data {
	int layers;
	int delta;
	bool equals;

	Data(int layers = 0, int delta = 0, int equals = true):
		layers(layers), delta(delta), equals(equals) {}

	Data operator+(Data const& other) const {
		return Data(
				value(),
				0,
				equals && other.equals && value() == other.value()
				);
	}

	int value() const {
		return layers + delta;
	}

	void resolve(Data& l, Data& r) {
		l.delta += delta;
		r.delta += delta;
		delta = 0;
	}
};

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
		std::cin >> x1 >> y1 >> x2 >> y2;

		rows.push_back(y1);
		rows.push_back(y2);

		data.emplace_back(x1, y1, y2, BEGIN);
		data.emplace_back(x2, y1, y2, END);
	}


	std::sort(rows.begin(), rows.end());

	Compress<int> compressed_rows(rows);


	LazySegmentTree<Data> tree(2 * n);

	std::sort(data.begin(), data.end());

	for (Boundary& boundary: data) {
		Segment segment(
				compressed_rows.compress(boundary.row_begin),
				compressed_rows.compress(boundary.row_end)
				);

		if (!tree.sum(segment).equals) {
			std::cout << '1';
			return 0;
		}

		if (boundary.type == BEGIN)
			tree.update(segment, [](Data& val) {val.delta++;});
		else
			tree.update(segment, [](Data& val) {val.delta--;});
	}

	std::cout << '0';
}
