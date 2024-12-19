#include "modules/Matrix.h"
#include "modules/FastIO.h"
#include <algorithm>
#include <array>

bool solution(Matrix<int>& square, std::vector<Matrix<char>>& pieces, std::array<bool, 5> used, int cnt) {

	Int2 first0 = square.size();

	for (Int2 t: square.bounds())
		if (square[t] == 0) {
			first0 = t;
			break;
		}

	if (first0 == square.size() && cnt == 5) return true;

	for (int i = 1; i <= 5; i++) {
		if (used[i - 1]) continue;

		Matrix<char>& piece = pieces[i - 1];

		int firstHash;

		for (int j = 0; j < piece.size().x; j++) {
			if (piece.at(j, 0) == '#') {
				firstHash = j;
				break;
			}
		}

		Int2 offset = first0;
		if (offset.x - firstHash < 0) continue;
		offset.x -= firstHash;

		if (offset.x + piece.size().x > square.size().x || offset.y + piece.size().y > square.size().y) continue;

		if (std::any_of(piece.bounds().begin(), piece.bounds().end(), [&square, &piece, offset](Int2 index) {
					return piece[index] == '#' && square[index + offset] != 0;
					})) continue;

		for (Int2 index: piece.bounds())
			if (piece[index] != '.') square[index + offset] = i;

		used[i - 1] = true;

		if (solution(square, pieces, used, cnt + 1)) return true;

		for (Int2 index: piece.bounds())
			if (piece[index] != '.') square[index + offset] = 0;

		used[i - 1] = false;
	}

	return false;
}

int main() {
	FastIO();
	int l;
	std::cin >> l;

	std::vector<Matrix<char>> pieces;
	pieces.reserve(5);

	for (int i = 0; i < 5; i++) {
		int n, m;
		std::cin >> n >> m;
		pieces.emplace_back(m, n);

		std::cin >> pieces[i];
	}

	Matrix<int> square(l, l);
	std::array<bool, 5> used {false, false, false, false, false};

	if (solution(square, pieces, used, 0)) {
		for (Int2 index: square.bounds()) {
			std::cout << square[index];

			if (index.x == square.size().x - 1)
				std::cout << '\n';
		}
	}
	else {
		std::cout << "gg";
	}
}
