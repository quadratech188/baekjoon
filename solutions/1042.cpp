#include "../modules/Matrix.h"
#include <iostream>

int main() {
	int n, m, l;
	std::cin >> n >> m >> l;

	std::string word;
	std::cin >> word;

	Matrix<char> board(m, n);
	std::cin >> board;

	Matrix<int> horizontal_sum(m + 1, n);
	Matrix<int> vertical_sum(m, n + 1);
	Matrix<int> results(m, n);

	for (Int2 index: board.bounds()) {
		if (board[index] != word[word.size() - 1]) continue;
		results[index] = 1;
	}

	for (int i = word.size() - 2; i >= 0; i--) {
		char prev = word[i + 1];
		int total = 0;
		for (Int2 index: board.bounds()) {
			if (board[index] != prev) continue;
			horizontal_sum[index + Int2::i()] = horizontal_sum[index] + results[index];
			vertical_sum[index + Int2::j()] = vertical_sum[index] + results[index];
			total += results[index];
		}

		char current = word[i];
		for (Int2 index: board.bounds()) {
			if (board[index] != current) continue;

			results[index] = total;

			static std::array<std::pair<Int2, Int2>, 12> h_lines = {
				std::make_pair(index + Int2::i(), Int2(m, index.y)),
				std::make_pair(index + Int2(2, 1), Int2(m, index.y + 1))
			};

			static std::array<Int2, 8> deltas = {
				Int2(1, 1),
				Int2(1, -1),
				Int2(-1, 1),
				Int2(-1, -1),
				Int2(2, 2),
				Int2(2, -2),
				Int2(-2, 2),
				Int2(-2, -2)
			};

			for (Int2 delta: deltas) {
				if (!board.bounds().contains(index + delta)) continue;
				results[index] -= results[index + delta];
			}
		}
	}

	int result = 0;
	for (Int2 index: board.bounds()) {
		if (board[index] != word[0]) continue;
		result += results[index];
	}

	std::cout << result;
}
