#include <iostream>
#include "../modules/Matrix.h"
#include "../modules/FastIO2.h"
int main() {
	int r, c;
	Fast::cin >> r >> c;

	Matrix<char> mine(c, r);
	for (Int2 index: mine.bounds())
		Fast::cin >> mine[index];

	Matrix<int> rightward(c, r);
	Matrix<int> leftward(c, r);

	for (int i = 0; i < c; i++) {
		rightward(i, 0) = mine(i, 0) - '1';
		leftward(i, 0) = mine(i, 0) - '1';
	}

	for (int j = 1; j < r; j++) {
		for (int i = 1; i < c; i++) {
			if (mine(i, j) == '1')
				rightward(i, j) = rightward(i - 1, j - 1) + 1;
			else
				rightward(i, j) = -1;
		}
		for (int i = 0; i < c - 1; i++) {
			if (mine(i, j) == '1')
				leftward(i, j) = leftward(i + 1, j - 1) + 1;
			else
				leftward(i, j) = -1;
		}
	}

	int max = -1;
	for (Int2 index: mine.bounds()) {
		int max_size = std::min(rightward[index], leftward[index]);

		for (int i = max + 1; i <= max_size; i++) {
			Int2 left_index = index + Int2(-i, -i);
			Int2 right_index = index + Int2(i, -i);
			if (!mine.bounds().contains(left_index)) continue;
			if (leftward[left_index] < i) continue;
			if (!mine.bounds().contains(right_index)) continue;
			if (rightward[right_index] < i) continue;
			max = i;
		}
	}

	std::cout << max + 1;
}
