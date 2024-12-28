#include <ios>
#include <vector>
#include <iostream>
#include <algorithm>
enum Train {
	X,
	Y
};

enum Border {
	BEGIN,
	END
};

struct Data {
	Data(int pos, Train train, Border border): pos(pos), train(train), border(border) {}

	int pos;
	Train train;
	Border border;
};

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);

	int n;
	std::cin >> n;

	std::vector<Data> x_borders;
	x_borders.reserve(2 * n);

	for (int i = 0; i < n; i++) {
		int x, y;
		std::cin >> x >> y;

		x_borders.emplace_back(x - 1, X, BEGIN);
		x_borders.emplace_back(y, X, END);
	}

	int m;
	std::cin >> m;

	std::vector<Data> y_borders;
	y_borders.reserve(2 * m);

	for (int i = 0; i < m; i++) {
		int z, w;
		std::cin >> z >> w;

		y_borders.emplace_back(z - 1, Y, BEGIN);
		y_borders.emplace_back(w, Y, END);
	}

	std::vector<std::pair<int, int>> data;

	for (Data& x_border: x_borders) {
		for (Data& y_border: y_borders) {
			int offset = x_border.pos + y_border.pos;

			int delta;

			if (x_border.border == BEGIN) {
				if (y_border.border == BEGIN)
					delta = 1;
				else
					delta = -1;
			}
			else {
				if (y_border.border == BEGIN)
					delta = -1;
				else
				 	delta = 1;
			}

			data.emplace_back(offset, delta);
		}
	}
	std::sort(data.begin(), data.end());

	int max = 0;
	int max_offset;
	int last_offset = data[0].first;

	int sum = 0;
	int slope = 0;

	for (auto& border: data) {
		sum += slope * (border.first - last_offset);
		slope += border.second;

		if (sum > max) {
			max_offset = border.first;
			max = sum;
		}

		last_offset = border.first;
	}

	std::cout << max_offset;
}
