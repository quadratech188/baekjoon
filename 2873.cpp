#include <iostream>
#include "modules/Matrix.h"

int main() {
	int r, c;
	std::cin >> r >> c;

	if (r % 2 == 1) {
		int garbage;
		for (int i = 0; i < r * c; i++) std::cin >> garbage;

		bool left = true;

		for (int i = 0; i < r - 1; i++) {
			if (left)
				for (int i = 0; i < c - 1; i++) std::cout << 'R';
			else
			 	for (int i = 0; i < c - 1; i++) std::cout << 'L';
			std::cout << 'D';
			left = !left;
		}
		for (int i = 0; i < c - 1; i++) std::cout << 'R';
		return 0;
	}
	if (c % 2 == 1) {
		int garbage;
		for (int i = 0; i < r * c; i++) std::cin >> garbage;

		bool down = true;

		for (int i = 0; i < c - 1; i++) {
			if (down)
				for (int i = 0; i < r - 1; i++) std::cout << 'D';
			else
			 	for (int i = 0; i < r - 1; i++) std::cout << 'U';;
			std::cout << 'R';
			down = !down;
		}
		for (int i = 0; i < r - 1; i++) std::cout << 'D';
		return 0;
	}
	return 0;
}
