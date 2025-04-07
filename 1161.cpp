#include <algorithm>
#include <iostream>
#include <istream>
#include "modules/LazySegmentTree.h"
#include "modules/Input.h"

struct Max {
	int update;
	int max;

	Max operator+(Max const& other) const {
		return Max {0, std::max(value(), other.value())};
	}

	int value() const {
		return update + max;
	}

	void resolve(Max& l, Max& r) {
		l.update += update;
		r.update += update;
		max += update;
		update = 0;
	}
};

struct Passengers {
	int count, start, end;

	bool operator<(Passengers const& other) const {
		if (start != other.start) return start < other.start;
		return end < other.end;
	}
};

std::istream& operator>>(std::istream& is, Passengers& p) {
	is >> p.start >> p.end >> p.count;
	return is;
}

int main() {
	int k, n, c;
	std::cin >> k >> n >> c;

	std::vector<Passengers> passengers = Input::toVec<Passengers>(k);

	std::sort(passengers.begin(), passengers.end());

	LazySegmentTree<Max> tree(n);

	int count = 0;
	for (auto& p: passengers) {
		int max = tree.sum(p.start - 1, p.end).value();

		int delta = std::min(p.count, c - max);

		if (delta > 0)
			tree.update(p.start - 1, p.end, [delta](Max& m) {m.update += delta;});

		count += delta;
	}

	std::cout << count;
}
