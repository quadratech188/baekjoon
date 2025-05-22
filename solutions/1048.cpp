#include "../modules/Matrix.h"
#include "../modules/ModInt.h"
#include "../modules/FastIO.h"
#include <iostream>

int main() {
	FastIO();
	int n, m, l;
	std::cin >> n >> m >> l;

	std::string word;
	std::cin >> word;

	Matrix<char> board(m, n);
	std::cin >> board;

	Matrix<sm32_1e9_7> horizontal_sum(m + 1, n);
	Matrix<sm32_1e9_7> vertical_sum(m, n + 1);
	Matrix<sm32_1e9_7> results(m, n);

	for (Int2 index: board.bounds()) {
		if (board[index] != word[word.size() - 1]) continue;
		results[index] = 1;
	}

	for (int i = word.size() - 2; i >= 0; i--) {
		char prev = word[i + 1];
		sm32_1e9_7 total = 0;
		for (Int2 index: board.bounds()) {
			if (board[index] == prev) {
				horizontal_sum[index + Int2::i()] = horizontal_sum[index] + results[index];
				vertical_sum[index + Int2::j()] = vertical_sum[index] + results[index];
				total += results[index];
			}
			else {
				horizontal_sum[index + Int2::i()] = horizontal_sum[index];
				vertical_sum[index + Int2::j()] = vertical_sum[index];
			}
		}

		char current = word[i];
		for (Int2 index: board.bounds()) {
			if (board[index] != current) continue;

			results[index] = total;

			std::array<std::pair<Int2, Int2>, 6> h_lines = {
				std::make_pair(Int2(0, index.y - 1), index + Int2(-2, -1)),
				std::make_pair(Int2(0, index.y), index + Int2(-2, 0)),
				std::make_pair(Int2(0, index.y + 1), index + Int2(-2, 1)),
				std::make_pair(Int2(index + Int2(3, -1)), Int2(m, index.y - 1)),
				std::make_pair(Int2(index + Int2(3, 0)), Int2(m, index.y)),
				std::make_pair(Int2(index + Int2(3, 1)), Int2(m, index.y + 1))
			};

			for (auto const& line: h_lines) {
				if (!horizontal_sum.bounds().contains(line.first)) continue;
				if (!horizontal_sum.bounds().contains(line.second)) continue;

				results[index] -= (horizontal_sum[line.second] - horizontal_sum[line.first]);
			}

			std::array<std::pair<Int2, Int2>, 6> v_lines = {
				std::make_pair(Int2(index.x - 1, 0), index + Int2(-1, -2)),
				std::make_pair(Int2(index.x, 0), index + Int2(0, -2)),
				std::make_pair(Int2(index.x + 1, 0), index + Int2(1, -2)),
				std::make_pair(Int2(index + Int2(-1, 3)), Int2(index.x - 1, n)),
				std::make_pair(Int2(index + Int2(0, 3)), Int2(index.x, n)),
				std::make_pair(Int2(index + Int2(1, 3)), Int2(index.x + 1, n)),
			};

			for (auto const& line: v_lines) {
				if (!vertical_sum.bounds().contains(line.first)) continue;
				if (!vertical_sum.bounds().contains(line.second)) continue;

				results[index] -= (vertical_sum[line.second] - vertical_sum[line.first]);
			}

			for (Int2 delta: Range2(Int2(-2, -2), Int2(3, 3))) {
				if (!results.bounds().contains(index + delta)) continue;
				results[index] -= (horizontal_sum[index + delta + Int2::i()] - horizontal_sum[index + delta]);
			}
		}
	}

	sm32_1e9_7 result = 0;
	for (Int2 index: board.bounds()) {
		if (board[index] != word[0]) continue;
		result += results[index];
	}

	std::cout << result;
}
