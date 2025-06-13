#include "../modules/Matrix.h"
#include "../modules/Math.h"
#include "../modules/FastIO2.h"
#include "../modules/FastIO.h"

uint N;

Int2 starting_point(Int2 dir) {
	if (dir == Int2(1, 0))
		return Int2(0, 0);
	if (dir == Int2(0, 1))
		return Int2(N - 1, 0);
	if (dir == Int2(-1, 0))
		return Int2(N - 1, N - 1);
	if (dir == Int2(0, -1))
		return Int2(0, N - 1);

	__builtin_unreachable();
}

void compress(Matrix<int>& board, Int2 dir) {
	Int2 search_dir = dir * -1;

	Int2 start = starting_point(search_dir);

	for (uint column = 0; column < N; column++) {
		Int2 column_start = start + search_dir.rotate(Math::pi / 2) * column;
		Int2 last_available = column_start;

		for (uint row = 0; row < N; row++) {
			Int2 index = column_start + search_dir * row;

			if (board[index] == 0) continue;

			int value = board[index];
			board[index] = 0;
			board[last_available] = value;

			last_available += search_dir;
		}
	}
}

void merge(Matrix<int>& board, Int2 dir) {
	Int2 search_dir = dir * -1;

	Int2 start = starting_point(search_dir);

	for (uint column = 0; column < N; column++) {
		Int2 column_start = start + search_dir.rotate(Math::pi / 2) * column;

		for (uint row = 0; row < N - 1; row++) {
			Int2 index = column_start + search_dir * row;

			if (board[index] == 0) continue;

			Int2 next_index = index + search_dir;

			if (board[index] == board[next_index]) {
				board[index] *= 2;
				board[next_index] = 0;
			}
		}
	}
}

void play(Matrix<int>& board, Int2 dir) {
	compress(board, dir);
	merge(board, dir);
	compress(board, dir);
}

int search(Matrix<int>& board, uint plays_left) {
	if (plays_left == 0) {
		int result = 0;

		for (Int2 index: board.bounds())
			result = std::max(result, board[index]);

		return result;
	}

	static Int2 moves[4] = {
		Int2(0, 1),
		Int2(1, 0),
		Int2(0, -1),
		Int2(-1, 0)
	};

	int result = 0;

	for (Int2 move: moves) {
		Matrix<int> copy = board;
		play(copy, move);
		result = std::max(result, search(copy, plays_left - 1));
	}

	return result;
}

int main() {
	FastIO();
	Fast::cin >> N;
	Matrix<int> board(N, N);
	Fast::cin >> board;

	std::cout << search(board, 5);
}
