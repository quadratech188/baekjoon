#include <algorithm>
#include <vector>
#include <iostream>

enum Direction {
	R0, // 0 deg
	R1, // 90 deg
	R2, // 180 deg
	R3, // 270 deg
};

template <typename T>
struct RotatableBoard {
	RotatableBoard(std::vector<std::vector<T>> board) {
		this->board = board;
		this->size = board.size();
		this->direction = R0;
	}

	std::vector<std::vector<T>> board;
	size_t size;
	Direction direction;

	T& operator[] (std::pair<size_t, size_t> index) {
		switch(this->direction) {
			case R0:
				return this->board[index.first][index.second];
			case R1:
				return this->board[index.second][this->size - 1 - index.first];
			case R2:
				return this->board[this->size - 1 - index.first][this->size - 1 - index.second];
			case R3:
				return this->board[this->size - 1 - index.second][index.first];
		}
	}

	friend std::ostream& operator<<(std::ostream& os, RotatableBoard<T>& board) {
		for (auto& row: board.board) {
			for (auto& val: row) {
				std::cout << val << '\t';
			}
			std::cout << std::endl;
		}
		return os;
	}
};

struct G2048: RotatableBoard<int> {
	G2048(std::vector<std::vector<int>> values): RotatableBoard(values) {};

	void merge() {
		for (size_t row = 0; row < this->size; row++) {
			for (size_t column = 0; column < this->size - 1; column++) {
				if ((*this)[{row, column}] == (*this)[{row, column + 1}]) {

					(*this)[{row, column}] *= 2;
					(*this)[{row, column + 1}] = 0;
				}
			}
		}
	}

	void squash() {
		for(size_t row = 0; row < this->size; row++) {
			size_t available = 0;
			for (size_t column = 0; column < this->size; column++) {
				if ((*this)[{row, column}] != 0) {
					(*this)[{row, available}] = (*this)[{row, column}];
					available ++;
				}
			}

			for (size_t column = available; column < this->size; column++) {
				(*this)[{row, column}] = 0;
			}
		}
	}
	void play() {
		squash();
		merge();
		squash();
	}
};

int solution(int left, G2048 game) {
	if (left == 0) {
		int result = 0;

		for (auto& row: game.board) {
			int max = *std::max_element(row.begin(), row.end());
			result = std::max(max, result);
		}

		return result;
	}

	const Direction directions[] = {R0, R1, R2, R3};

	int result = 0;

	for (Direction direction: directions) {
		G2048 copy = game;
		copy.direction = direction;
		copy.play();
		result = std::max(result, solution(left - 1, copy));
	}

	return result;
}

int main() {
	int n;
	std::cin >> n;
	
	std::vector<std::vector<int>> values;
	values.resize(n);

	for (int row = 0; row < n; row++) {
		values[row].resize(n);
		for (int column = 0; column < n; column++) {
			std::cin >> values[row][column];
		}
	}

	G2048 game(values);

	/*
	while (true) {
		char input;
		std::cin >> input;

		switch(input) {
			case 'h':
				game.direction = R0;
				break;
			case 'j':
				game.direction = R3;
				break;
			case 'k':
				game.direction = R1;
				break;
			default:
				game.direction = R2;
		}

		game.play();

		std::cout << game;
	}
	*/

	std::cout << solution(5, game);
}
